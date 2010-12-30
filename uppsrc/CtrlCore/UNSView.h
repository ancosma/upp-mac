#ifndef __UNSVIEW_H__
#define __UNSVIEW_H__

namespace Upp {
	
	class Ctrl;
	
} 

using UPP::Ctrl;


@interface UNSView : NSView
{
	UPP::Ctrl * pCtrl_;
}
//+ (id) initialize;
//- (id) initWithOwner:(UPP::UppCocoaWindowImpl*)owner;  //what if we used a separate wrapped owner...?
-(void)setOwnerCtrl:(UPP::Ctrl *)owner;  //what if we used a separate owner...?


@end



#endif