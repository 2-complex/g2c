#include "parse.h"

#include <stack>

#include <stdlib.h>

using namespace parse;
using namespace std;

bool parse::charInString(char c, const char* s)
{
    for(int i = 0; s[i]; i++)
        if( c == s[i] )
            return true;
    return false;
}


Node::~Node()
{
    clearChildren();
    if(data.value)
        delete data.value;
}

void Node::clearChildren()
{
    for(vector<Node*>::iterator itr = children.begin(); itr!=children.end(); itr++)
        delete *itr;
    children.clear();
}

void Node::parse()
{
    const char* pairs = "{}()[]\'\'\"\"";
    std::stack<char> q;
    
    clearChildren();
    
    // Start with an invisible separator character before the whole thing.
    vector<int> separators;
    separators.push_back(begin-1);
    
    bool escaped = false;

    for(int i=begin; i<end; i++)
    {
        int level = q.size();
        char c = s[i];
        
        char open_char = 0;
        if( level )
            open_char = q.top();
        
        if( level == 0 )
        {
            if( c == ',' || c == ':' )
                separators.push_back(i);
        }
        
        // If we're within a quote, we look for the other quote only.
        if(level > 0 && (open_char == '\'' || open_char == '"'))
        {
            if( !escaped && c == open_char )
                q.pop();
            
            if( !escaped && c == '\\' )
                escaped = true;
            else
                escaped = false;
        }
        else
        {
            // Look for any opening character.
            for(unsigned int j = 0; j < strlen(pairs); j+=2)
            {
                char begin_c = pairs[j];
                char end_c = pairs[j+1];
                
                // Note if begin_c == end_c, this will begin it.
                if( c==begin_c )
                    q.push(c);
                else if( c==end_c )
                    q.pop();
            }
        }
    }
    
    separators.push_back(end);
    
    int n = separators.size();
    if( n==2 )
    {
        // We pretend there's a comma before the beginning.
        int a = separators[0]+1, b = separators[1];
        
        // Strip whitespace.
        while(a < b && charInString(s[a], " \r\n\t")) a++;
        while(a < b && charInString(s[b-1], " \r\n\t")) b--;
        
        if(b-a > 0)
        {
            // If it's a string.
            if( s[a] == s[b-1] && (s[a] == '"' || s[a] == '\'') )
            {
                string r;
                bool escaped = false;
                for(int j = a+1; j < b-1; j++)
                {
                    char c = s[j];
                    
                    if( !escaped && c=='\\' )
                    {
                        escaped = true;
                        continue;
                    }
                    
                    if( escaped )
                    {
                        switch(c)
                        {
                            case 'n':
                                r += '\n';
                            break;
                            
                            case 'r':
                                r += '\r';
                            break;
                            
                            case 't':
                                r += '\t';
                            break;
                            
                            default:
                                r += c;
                            break;
                        }
                    }
                    else
                        r += c;
                    
                    escaped = false;
                }
                data.s = r;
                type = kString;
            }
            
            char num_chars[] = "-.0123456789";
            bool is_num = true;
            bool is_float = false;
            
            // Iterate through each character in the substring, make sure it's a
            // digit or a dot.  Then we know it's a number.
            for(int j = a; j < b; j++)
            {
                if(!charInString(s[j], num_chars))
                {
                    is_num = false;
                    break;
                }
                if(s[j] == '.')
                {
                    is_float = true;
                }
            }
            
            if( is_num )
            {
                string r;
                for(int j = a; j < b; j++)
                {
                    char buffer[2] = {s[j], 0};
                    r += buffer;
                }
                if(is_float)
                {
                    data.x = atof(r.c_str());
                    data.i = data.x;
                    type = kFloat;
                }
                else
                {
                    data.i = atoi(r.c_str());
                    data.x = data.i;
                    type = kInt;
                }
            }
            
            // If it's a node.
            if( (s[a] == '{' && s[b-1] == '}') ||
                (s[a] == '[' && s[b-1] == ']') ||
                (s[a] == '(' && s[b-1] == ')') )
            {
                char front = s[a];
                a++;
                b--;
                
                if(front == '{') type = kObject;
                if(front == '[') type = kList;
                if(front == '(') type = kTuple;
                
                // Clear whitespace.
                while(a < b && charInString(s[a], " \r\n\t")) a++;
                while(a < b && charInString(s[b-1], " \r\n\t")) b--;
                if( a < b )
                {
                    Node* n = new Node(s, a, b);
                    if(n->type == kNode)
                    {
                        children = n->children;
                        n->children.clear();
                        delete n;
                    }
                    else
                    {
                        children.push_back(n);
                    }
                }
            }
        }
    }
    else
    {
        Node* lastNode = NULL;
        for(int i = 0; i < n-1; i++)
        {
            int a = separators[i]+1, b = separators[i+1];
            
            bool second_of_pair = lastNode && s[a-1]==':';
            
            // Strip whitespace.
            while(a < b && charInString(s[a], " \r\n\t")) a++;
            while(a < b && charInString(s[b-1], " \r\n\t")) b--;
            
            if(b-a > 0)
            {
                Node* n = new Node(s, a, b);
                if(second_of_pair)
                {
                    lastNode->data.value = n;
                }
                else
                {
                    children.push_back(n);
                }
                lastNode = n;
            }
        }
    }
}

void Node::display() const
{
    print();
    cellolog( "\n" );
}

void Node::print() const
{
    switch(type)
    {
        case kInt:
            cellolog( "%d", data.i );
        break;
        
        case kFloat:
            cellolog( "%f", data.x );
        break;
        
        case kString:
            cellolog( "'%s'", data.s.c_str() );
        break;
        
        case kObject:
        case kList:
        case kTuple:
        default:
            char front = 0, back = 0;
            switch(type)
            {
                case kObject:
                    front = '{';
                    back = '}';
                break;
                case kList:
                    front = '[';
                    back = ']';
                break;
                case kTuple:
                    front = '(';
                    back = ')';
                break;
                
                case kNode:
                case kInt:
                case kFloat:
                case kString:
                break;
            }
            if(front)
                putchar(front);
            for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
            {
                Node* n = *itr;
                n->print();
                if(n->data.value)
                {
                    putchar(':');
                    n->data.value->print();
                }
                cellolog( "," );
            }
            if(back)
                putchar(back);
        break;
    }
}


