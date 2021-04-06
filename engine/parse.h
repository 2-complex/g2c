#ifndef _PARSE_
#define _PARSE_

#include "log.h"

#include <vector>
#include <string>

#include <string.h>

namespace parse {

class Node;

class Data {
public:
    Data() : i(0), x(0.0), value(NULL) {}
    virtual ~Data() {}
    int i;
    double x;
    std::string s;
    Node* value;
};

enum Type {
    kNode = 0,
    kInt = 1,
    kFloat,
    kString,
    kObject,
    kList,
    kTuple
};

class Node {
public:
    Node() :
        s(NULL),
        begin(0),
        end(0),
        type(kNode) {s = (char*)"";}
    
    Node(const char* s) : s(s),
        type(kNode) {begin = 0; end = strlen(s); parse();}
    
    Node(const char* s, int begin, int end) : 
        s(s),
        begin(begin),
        end(end),
        type(kNode) {parse();}
    
    virtual ~Node();
    
    const char* s;
    int begin;
    int end;
    
    Type type;
    Data data;
    
    std::vector<Node*> children;
    
    void display() const;
    void print() const;
    
protected:
    void parse();
    void clearChildren();
};

bool charInString(char c, const char* s);
    
}

#endif

