#ifndef __UNSAPPLICATIONDELEGATE_H__
#define __UNSAPPLICATIONDELEGATE_H__

#include "CtrlCore.h"

#if (MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5)  //I am not sure if that should be the other way round
@interface UNSApplicationDelegate : NSObject <NSApplicationDelegate> {
#else
@interface UNSApplicationDelegate : NSObject  {
#endif

	WindowRef windowref_;
	
//	NSLog(@"UNSApplicationDelegate : NSObject <NSApplicationDelegate>");
}
-(void) setWindowRef:(WindowRef) _windowref;
@end
	
#endif