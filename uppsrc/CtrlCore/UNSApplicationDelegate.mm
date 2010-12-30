
#include "CtrlCore.h"

//using UPP::Ctrl;

@implementation UNSApplicationDelegate

-(void) applicationWillFinishLaunching:(NSNotification *)aNotification
{
	Ctrl::CocoaMenu();  //To put a callback in here?
}

-(void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
	
//	NSLog(@"applicationDidFinishLaunching");
	Ctrl::CocoaStart();  //should be named AfterCocoaDidFinishLaunching?
	
	NSLog(@"applicationDidFinishLaunching start EventLoop in =%@", windowref_);	
//	UPP::Ctrl::EventLoop( UPP::Ctrl::CtrlFromWindowRef(windowref_) );  //works
//	UPP::Ctrl::EventLoop( (UPP::TopWindow *)(windowref_) );
	
//	NSLog(@"applicationDidFinishLaunching AFTER CTRL LOOP!!!");
	
}
-(void) applicationWillTerminate:(NSNotification *)aNotification
{
	NSLog(@"NSApplicationWillTerminate from Delegate");
	Ctrl::CocoaExit();
}

-(void) setWindowRef:(WindowRef) _windowref
{
	windowref_ = _windowref;
}

@end



