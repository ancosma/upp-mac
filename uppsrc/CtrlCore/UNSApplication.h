#ifndef __UNSAPPLICATION_H__
#define __UNSAPPLICATION_H__

	
@interface UNSApplication : NSApplication
{
}
-(void)sendEvent:(NSEvent*)event;
-(void)sendSuperEvent:(NSEvent*)event;
-(void)keyDown:(NSEvent *)event;
//-(void) discardEventsMatchingMask:(unsigned int)mask
//					  beforeEvent:(NSEvent *)lastEvent;
	//-(NSMenu*)applicationDockMenu:(NSApplication *)sender;
	//-(BOOL)application: (NSApplication*) app openFile: (NSString*)file;
	//-(void)application: (NSApplication*) app openFiles: (NSArray*)files;
	//-(BOOL)application: (NSApplication*) app printFile: (NSString*)file;
	//-(NSApplicationPrintReply)application: (NSApplication *) app printFiles:(NSArray *)files withSettings: (NSDictionary *)printSettings showPrintPanels:(MacOSBOOL)bShowPrintPanels;
	//-(NSApplicationTerminateReply)applicationShouldTerminate: (NSApplication *) app;
	//-(void)systemColorsChanged: (NSNotification*) pNotification;
	//-(void)screenParametersChanged: (NSNotification*) pNotification;
	//-(void)scrollbarVariantChanged: (NSNotification*) pNotification;
	//-(void)scrollbarSettingsChanged: (NSNotification*) pNotification;
	//-(void)addFallbackMenuItem: (NSMenuItem*)pNewItem;
	//-(void)removeFallbackMenuItem: (NSMenuItem*)pOldItem;
	//-(void)addDockMenuItem: (NSMenuItem*)pNewItem;
	//-(void)applicationWillBecomeActive: (NSNotification *)pNotification;
	//-(void)applicationWillResignActive: (NSNotification *)pNotification;
	//-(BOOL)applicationShouldHandleReopen: (NSApplication*)pApp hasVisibleWindows: (MacOSBOOL)bWinVisible;
	//-(void)setDockIconClickHandler: (NSObject*)pHandler;
	//-(void)cycleFrameForward: (AquaSalFrame*)pCurFrame;
	//-(void)cycleFrameBackward: (AquaSalFrame*)pCurFrame;
@end

#endif