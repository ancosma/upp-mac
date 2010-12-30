//#import "UppNSWindow.h"
#include "CtrlCore.h"

using UPP::Ctrl;

@implementation UNSWindow
//- (id) initWithOwner:(UppCocoaWindowImpl*)owner
/*
 + (id) initialize
 {
 */

- (void)setOwnerCtrl:(UPP::Ctrl*)owner
{
	pCtrl_ = owner;
/*
 NSScreen* mainScreen = [[NSScreen screens] objectAtIndex:0];
 NSRect topframe = NSMakeRect(10.0f, 10.0f, 800.0f, 600.0f);
 NSUInteger styleMask =  NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask; //  NSBorderlessWindowMask;
 
 NSRect windowRect = [NSWindow contentRectForFrameRect:topframe styleMask:styleMask];
 
 
 //	self = [super initWithContentRect:[mainScreen frame]
 self = [super initWithContentRect:windowRect
 styleMask:styleMask
 backing:NSBackingStoreBuffered
 defer:NO
 screen:mainScreen];
 if (self) {
 //owner_ = owner;
 [self setAcceptsMouseMovedEvents:YES];
 //To receive focus changed notifications:
 [self setDelegate:self];
 }
 return self;
 */
 
 }


- (BOOL)acceptsFirstResponder
{
	NSLog(@"Accepts First Responder");	
	return YES;
}

- (BOOL)becomeFirstResponder
{
	NSLog(@"Become First Responder");
	return YES;
	
}
- (BOOL)resignFirstResponder
{
	NSLog(@"Resigns First Responder");
	return YES;
}
- (BOOL)canBecomeKeyWindow
{
	return YES;  //shall we create a separate variable to switch it?
}
- (BOOL)canBecomeMainWindow
{
	return YES;
}


-(void)keyDown:(NSEvent *)event
{
	NSLog(@"keyDown:event = %@", event);	
	//	[super sendEvent:event];
}

- (void)keyUp:(NSEvent*)event {
	NSLog(@"keyUp = %@", event);
	//	[super sendEvent:event];
	//	owner_->DispatchKeyEvent(event);	
	
}

/*
 - (void)mouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 [super sendEvent:event];
 }
 
 - (void)mouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseEntered:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseExited:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseMoved:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)windowDidResignKey:(NSNotification*)notification {
 //	owner_->DispatchFocusLostEvent();
 }
 
 - (void)windowDidResignMain:(NSNotification*)notification {
 //	owner_->DispatchFocusLostEvent();
 }
 */

/*
- (void)drawRect:(NSRect)rect
{
//It would be good to recieve paint events here and avoid using NVView
// but I couldnt get them
}
*/
@end

/*
namespace Upp {
UppCocoaWindow::UppCocoaWindow()
{

}


UppCocoaWindow::~UppCocoaWindow()
{
	
}

	
} //namespace
*/
