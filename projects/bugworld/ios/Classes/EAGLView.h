
#import <UIKit/UIKit.h>

#import "ESSetup.h"

#include "sprites.h"
#include "iosresourcebank.h"
#include "app.h"

#include <map>

class Insider
{
public:
	cello::App* app;
	cello::World* world;
	cello::IOSResourceBank* bank;
	cello::Renderer* renderer;
	std::map<UITouch*, int> touchIndexMap;
};

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
@private
	id <ESSetup> setup;
	
	int mWidth;
	int mHeight;
	
	BOOL firstReshape;
	BOOL animating;
	BOOL displayLinkSupported;
	NSInteger animationFrameInterval;
	
	// Use of the CADisplayLink class is the preferred method for controlling animation timing.
	// CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
	// The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
	// isn't available.
	id displayLink;
    NSTimer *animationTimer;
	
	Insider* insider;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void) startAnimation;
- (void) stopAnimation;
- (void) initWorld;
- (void) drawView:(id)sender;


- (void) resizeWithWidth:(int)width height:(int)height;

@end

