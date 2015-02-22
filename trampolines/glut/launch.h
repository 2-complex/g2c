
#ifndef _LAUNCH_
#define _LAUNCH_

#include "app.h"

void launch(g2c::App* app);

#define LAUNCH(AppType) \
int main() \
{ \
    AppType app; \
    launch(&app); \
    return 0; \
}

#endif


