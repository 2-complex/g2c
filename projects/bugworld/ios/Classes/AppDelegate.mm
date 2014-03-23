
#import "AppDelegate.h"
#import "EAGLView.h"
#import "EAGLViewController.h"

@implementation AppDelegate

@synthesize window;
@synthesize glView;

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	// Forbid the screen from sleeping.
	application.idleTimerDisabled = YES;
	
	// Hide the status bar (that thing at the top with the battery life etc)
	[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
	
	viewController = [[EAGLViewController alloc] init];
	viewController.wantsFullScreenLayout = YES;
	viewController.view.multipleTouchEnabled = YES;
	
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
	[glView startAnimation];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
	[glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) dealloc
{
	[window release];
	[glView release];
	
	[super dealloc];
}

@end

