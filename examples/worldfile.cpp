
#include "sprites.h"

#include "scrollenvironment.h"

#include <stdio.h>

#include <string>

using namespace std;
using namespace g2c;

class SpriteEnvironment : public ScrollEnvironment
{
public:
	SpriteEnvironment();
	virtual ~SpriteEnvironment();
	
	bool forwardKeyboard;
	
	void initWithPath(const char* filename);

protected:
	World world;
	mutable RendererGL1 renderer;

	Bank* bank;
	AudioPlayer* player;

	virtual void enables();
	
	virtual void reshape(int x, int y);
	virtual void draw() const;
	
	virtual bool mouseDown(const Vec2& C);
	virtual void mouseDragged(const Vec2& C);
	virtual void mouseUp(const Vec2& C);
	
	virtual void keyboard(unsigned char inkey);
	
private:
	std::string filename;
	
	int sound_index;
	bool editMode;
	Actor* current;
	Vec2 lastLoc;
};



SpriteEnvironment::SpriteEnvironment() : sound_index(0), forwardKeyboard(false), current(NULL),
    editMode(false)
{
    world.bank = bank;
    world.initSound(&player);
    Mesh::renderer = &renderer;
}

SpriteEnvironment::~SpriteEnvironment()
{
}

void SpriteEnvironment::enables()
{
    Environment::enables();

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);                        // enables a light (we need that aparently)
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);  // sets lighting to one-sided

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE); // ?

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    renderer.init();
}

void SpriteEnvironment::initWithPath(const char* filename)
{
    bank->initSerializableWithPath(&world, filename);
    this->filename = filename;
}

void SpriteEnvironment::reshape(int w, int h)
{
    ScrollEnvironment::reshape(w, h);
    renderer.projection = getProjection();
}

void SpriteEnvironment::draw() const
{
    world.draw();

    if(Sprite::drawLines)
    {
        glDisable(GL_TEXTURE_2D);
            glColor3f(0,1,1);
            glBegin(GL_LINE_LOOP);
                /*
                glVertex3f(0,0,1);
                glVertex3f(320,0,1);
                glVertex3f(320,480,1);
                glVertex3f(0,480,1);
                */
                glVertex3f(0,0,1);
                glVertex3f(1024,0,1);
                glVertex3f(1024,768,1);
                glVertex3f(0,768,1);
            glEnd();
        glEnable(GL_TEXTURE_2D);
    }
}

bool SpriteEnvironment::mouseDown(const Vec2& C)
{
    if( editMode )
    {
        Vec2 localC = C - center;
        lastLoc = localC;
        current = world.actorInClick(localC);

        if( current )
        {
            printf( "selected: %s\n", current->name.c_str() );
            return true;
        }
        return false;
    }

    return world.mouseDown(C);
}

void SpriteEnvironment::mouseDragged(const Vec2& C)
{
    if( editMode )
    {
        Vec2 localC = C - center;
        if( current )
        {
            Mat4 m = current->getWorldMatrix();
            Vec2 delta = (localC - lastLoc);
            Vec4 v = m.inverse()*Vec4(delta.x, delta.y, 0, 0);
            delta.set(v.x, v.y);

            current->position += delta;
        }

        lastLoc = localC;
    }
    else
        world.mouseDragged(C);
}

void SpriteEnvironment::mouseUp(const Vec2& C)
{
    if( editMode )
        mouseDragged(C);
    else
        world.mouseUp(C);
}

void SpriteEnvironment::keyboard(unsigned char inkey)
{
    if( forwardKeyboard )
    {
        world.keyboard(inkey);
        return;
    }

    switch(inkey)
    {
        case 'e':
        {
            editMode = !editMode;
            if( editMode )
                printf( "Edit mode on.  Hit shift-S to save.\n" );
            else
                printf( "Edit mode off.\n" );
        }
        break;

        case 'S':
        {
            FILE* fp = fopen(filename.c_str(), "w");
            if( fp )
            {
                printf( "writing to file: %s\n", filename.c_str() );
                string s = world.serialize();
                fwrite(s.c_str(), 1, s.size(), fp);
                fclose(fp);
            }
            else
                printf( "couldn't write to file." );
        }
        break;

        case 'p':
        {
            string s = world.serialize();
            printf("%s\n", s.c_str());
        }
        break;


        case 'l':
            Sprite::drawLines = !Sprite::drawLines;
        break;

        case 'c':
            center.display();
        break;

        case ' ':
        {
            int i = 0;
            int n = world.sounds.size();
            for(vector<Sound*>::iterator itr = world.sounds.begin();
                itr!=world.sounds.end();
                itr++)
            {
                if( i == sound_index )
                {
                    printf( "playing sound %s\n", (*itr)->name.c_str() );
                    world.playSound((*itr)->name);
                }
                i++;
            }

            if( n > 0 )
                sound_index = (sound_index+1)%n;
        }
        break;
    }

    if(current)
    {
        switch(inkey)
        {
            case '+':
            case '=':
                current->k += 0.01;
            break;

            case '-':
            case '_':
                current->k -= 0.01;
            break;

            case 'a':
                current->position.x-=0.1;
                break;
            case 'd':
                current->position.x+=0.1;
                break;
            case 's':
                current->position.y-=0.1;
                break;
            case 'w':
                current->position.y+=0.1;
                break;

            case 'r':
                current->frame+=1;
                break;
            case 'f':
                current->frame-=1;
                break;
        }
    }
}


class MySpriteEnvironment : public SpriteEnvironment
{
public:
	char* filename;

private:
	void init();
};

void MySpriteEnvironment::init()
{
	bank.initSerializableWithPath(&world, filename);
	printf( "%s\n", world.serialize().c_str() );
}

int main(int argc, char** args)
{
	MySpriteEnvironment e;

	e.filename = (char*)"bug.world";
	if( argc > 1 )
		e.filename = args[1];
	
	e.mainLoop();
	
	return 0;
}

