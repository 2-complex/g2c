#if defined(ANDROID)
    #include <android/log.h>
    #define cellolog(...)  __android_log_print(ANDROID_LOG_INFO, "cello", __VA_ARGS__)
    #define celloerror(...)  __android_log_print(ANDROID_LOG_ERROR, "cello", __VA_ARGS__)
#else
    #include <stdio.h>
    #define cellolog(...) printf(__VA_ARGS__)
    #define celloerror(...) fprintf(stderr,__VA_ARGS__)
#endif

