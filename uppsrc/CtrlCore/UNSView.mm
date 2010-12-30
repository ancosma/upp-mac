
//#import "UNSView.h"
#include "CtrlCore.h"

using UPP::Ctrl;

@implementation UNSView
- (void)setOwnerCtrl:(UPP::Ctrl*)owner
{
	pCtrl_ = owner;
}
- (void)rightMouseDown:(NSEvent*)event {
	//cocoa requires to forward right mouse button events?
	NSResponder* next = [self nextResponder];
	if (next != nil) {
		[next rightMouseDown:event];
	}
}

-(BOOL)isFlipped
{
	return YES;
}

- (id)initWithFrame:(NSRect)frameRect
{
	[super initWithFrame:frameRect];
	return self;
}

- (void)drawRect:(NSRect)rect
{
    CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];

	UPP::SystemDraw draw(context);

//	UpdateArea(draw, Rect(ps.rcPaint));
	NSLog(@"drawRect:(NSRect)rect= %d", (int)rect.origin.y);
	pCtrl_->UpdateArea(draw, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height) );
}
@end
