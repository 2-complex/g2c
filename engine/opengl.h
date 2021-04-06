#if IOS
    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES1/glext.h>
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>

#elif ANDROID
    #include <GLES/gl.h>
    #include <GLES/glext.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

#elif EMSCRIPTEN
    #include <SDL_opengles2.h>

#else // Assume a glut build.
    #define GLUT 1
    #ifdef __APPLE_CC__
        #include <glut/glut.h>
        #define initOpenGL() do{}while(0)
    #else
        #include <GL/glew.h>
        #include <GL/freeglut.h>
        #include <GL/freeglut_ext.h>
        #define initOpenGL() glewInit()
    #endif
#endif

