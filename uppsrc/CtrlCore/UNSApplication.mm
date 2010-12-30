
#include "CtrlCore.h"


@implementation UNSApplication

- (void)sendEvent:(NSEvent *)event
{
//	NSEventType eType = [pEvent type];
//    if( eType == NSApplicationDefined )
//        ->handleAppDefinedEvent( pEvent );
//    else if( eType == NSKeyDown && ([pEvent modifierFlags] & NSCommandKeyMask) != 0 )
//    {
	/*
        NSWindow* pKeyWin = [NSApp keyWindow];
        if( pKeyWin && [pKeyWin isKindOfClass: [UNSWindow class]] )
        {
				NSLog(@"UppNSApplication sendEvent:theEvent = %@", event);

		}
	*/
//	Ctrl::ProcessEvent(event);
	
//	NSLog(@"UppNSApplication sendEvent:theEvent = %@", event);
//	switch ([event type]) 
//	{
//		case NSKeyDown:
			//		    if ([event modifierFlags] & NSControlKeyMask) {
//		{
//			NSLog(@"test");
//			NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
//			[nc postNotificationName:@"shortCutKey" object:event];
//		}
//			break;
//		default:
//			break;
//	}
//	NSLog(@"event %@", event);
	[super sendEvent:event];
	
}

-(void)sendSuperEvent:(NSEvent*)event
{
    [super sendEvent: event];
}

-(void)keyDown:(NSEvent *)event
{
	NSLog(@"NSApplication keyDown:event = %@", event);	
	[super sendEvent:event];
}

//-(void) discardEventsMatchingMask:(unsigned int)mask
//					  beforeEvent:(NSEvent *)lastEvent
//{
//	NSLog(@"discardEventsMatchingMask:event = %@", lastEvent);	
//}

//-(void) applicationWillTerminate:(NSNotification *)aNotification
//{
//	NSLog(@"NSApplicationWillTerminate from application");
//	UPP::Ctrl::CocoaExit();
//}

@end