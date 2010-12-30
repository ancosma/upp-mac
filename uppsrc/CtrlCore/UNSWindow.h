#ifndef __UNSWINDOW_H__
#define __UNSWINDOW_H__

#include "CtrlCore.h"

namespace Upp {
	
	class Ctrl;
	
} 

using UPP::Ctrl;


@interface UNSWindow : NSWindow // <NSWindowDelegate> 
{
	UPP::Ctrl * pCtrl_;
}
//+ (id) initialize;
//- (id) initWithOwner:(UPP::UppCocoaWindowImpl*)owner;  //what if we used a separate owner...?
-(void)setOwnerCtrl:(UPP::Ctrl *)owner;  //what if we used a separate owner...?
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
- (void)keyUp:(NSEvent*)event;
- (void)keyUp:(NSEvent*)event;
/*
 - (void)mouseDown:(NSEvent*)event;
 - (void)mouseUp:(NSEvent*)event;
 - (void)mouseMoved:(NSEvent*)event;
 - (void)mouseDragged:(NSEvent*)event;
 - (void)mouseEntered:(NSEvent*)event;
 - (void)mouseExited:(NSEvent*)event;
 - (void)rightMouseDown:(NSEvent*)event;
 - (void)rightMouseDragged:(NSEvent*)event;
 - (void)rightMouseUp:(NSEvent*)event;
 - (void)otherMouseDown:(NSEvent*)event;
 - (void)otherMouseDragged:(NSEvent*)event;
 - (void)otherMouseUp:(NSEvent*)event;
 - (void)windowDidResignKey:(NSNotification*)notification;
 - (void)windowDidResignMain:(NSNotification*)notification;
 */
@end


#endif