
#ifndef _LAUNCH_
#define _LAUNCH_

#include "app.h"

g2c::App* getApp();

#define LAUNCH(AppType) \
    static App* app = new AppType; \
    g2c::App* getApp() {return app;}

#endif

