
#include <launch.h>

#include <stdio.h>

using namespace std;

GLuint programObject;
SDL_Surface* screen;

GLfloat vVertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};
GLint uniformOriginX, uniformOriginY, uniformZoom;


void initSDL(int width, int height)
{
    static bool initializedAlready = false;

    if( ! initializedAlready )
    {
        if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0 )
        {
            screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
            if (screen == NULL)
            {
                printf("Could not set video mode: %s", SDL_GetError());
            }
        }
        else
        {
            printf("Could not initialize SDL: %s", SDL_GetError());
        }
        initializedAlready = true;
    }
}

extern "C" void mouseDown(double x, double y)
{
    getApp()->mouseDown(Vec2(x,y));
}

extern "C" void mouseDragged(double x, double y)
{
    getApp()->mouseDragged(Vec2(x,y));
}

extern "C" void mouseUp(double x, double y)
{
    getApp()->mouseUp(Vec2(x,y));
}

extern "C" void init()
{
    printf("init\n");
    getApp()->init();
}

extern "C" void resize(int width, int height)
{
    initSDL(width, height);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, width, height);

    getApp()->resize(width, height);
}

extern "C" void step(double t)
{
    getApp()->step(t);
}

extern "C" void draw()
{
    getApp()->draw();
    SDL_GL_SwapBuffers();
}

