#include "emscriptentrampoline.h"

#include <emscripten.h>
#include <SDL_opengles2.h>
#include <SDL.h>

using namespace std;

// an example of something we will control from the javascript side
bool g_background_is_black = true;

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE g_toggle_background_color()
{
    g_background_is_black = !g_background_is_black;
}

EmscriptenTrampoline::EmscriptenTrampoline(App* app)
    : app(app)
{
}

EmscriptenTrampoline::~EmscriptenTrampoline()
{
}

void EmscriptenTrampoline::initWindow(const std::string& windowTitle, int width, int height)
{
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

static std::function<void()> loop;
static void main_loop() { loop(); }

void EmscriptenTrampoline::mainLoop()
{
    app->init();
    double clock = 0.0;

    loop = [&]
    {
        if( g_background_is_black )
        {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        }

        glClearDepthf(0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        app->step(clock);
        clock += 0.01;

        app->draw();

        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);
}

