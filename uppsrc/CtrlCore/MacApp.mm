#include "CtrlCore.h"

NAMESPACE_UPP

#ifdef  PLATFORM_COCOA

extern String lllg;
#define LLOG(x)   lllg=""; \
lllg<<x; \
NSLog(@"%s",(const char*)lllg);


void Ctrl::InitCocoa()
{
	GuiLock __;
//Ctrl *mw = GetMainWindow();
//	if(*mw)
//		WhenApplicationLoaded <<= callback(this, StartAfterApplicationLoaded);
//	InstallPanicMessageBox(sPanicMessageBox);
	
//	InitX11Draw(display);
//	InitTimer();
//	byte dummy[5];
//	Xbuttons = XGetPointerMapping(Xdisplay, dummy, 5);
	
//	Xeventtime = CurrentTime;
//	SetX11ErrorHandler();
//	if(GetIniKey(INI_PREFIX "X11_SYNCHRONIZE") == "1")
//		XSynchronize(Xdisplay, 1);
//	Vector<int> nets = GetPropertyInts(Xroot, XAtom("_NET_SUPPORTED"));
//	for(int i = 0; i < nets.GetCount(); i++)
//		_NET_Supported().Add(nets[i]);

	/*
	SetStdFont(Arial(12));
	
	ChSync();
	
	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_DragFullWindow_Write(1);
	GUI_PopUpEffect_Write(IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	GUI_DropShadows_Write(1);
	GUI_AltAccessKeys_Write(1);
	GUI_AKD_Conservative_Write(0);
	
	setlocale(LC_ALL, "en_US.utf8");
//	if(XSupportsLocale()) {
//		XSetLocaleModifiers("");
//		xim = XOpenIM(Xdisplay, NULL, NULL, NULL);
//	}
//	else {
//		xim = NULL;
//		LOG("IM unsupported!");
//	}
	
//	Csizeinit();
	*/
	NSLog(@"InitCocoa");
//	Ctrl::GetPrimaryWorkArea(); //remove after test
	
	 fprintf( stderr, "test fprintf InitCocoa \n" );
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	
	
	[UNSApplication sharedApplication];

	Ctrl::appname=[[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"] UTF8String];  //should we do our own bundle reading? & to find howto assign
	if (Ctrl::appname.IsEmpty())
			Ctrl::appname="Ctrl::appname-check CFBundleName";
	LLOG("Ctrl::appname="<<Ctrl::appname);

	
	[NSApp setDelegate:[[UNSApplicationDelegate alloc] init]];
	//	[NSApp setDelegate: NSApp];
	
	
	[pool release];
}

void Ctrl::CocoaMenu()
{
	/* 
	 Creates an initial cocoa menu.
	 If a nib file in your bundle is absent, AppKit tries to load an empty nib from somewhere (../Frameworks/? I forgotten where)
	 so maybe this way here we can create some useful items like those below?
	 In the future must be changed.
	 */
	NSMenu * mainMenu = [[NSMenu alloc] initWithTitle:@"MainMenu"];
//	NSMenu * mainMenu = new NSMenu;
//	[mainMenu initWithTitle:@"MainMenu"];
	
	
	NSMenuItem * menuItem;
	NSMenu * submenu;
	
	
	menuItem = [mainMenu addItemWithTitle:@"Apple" action:NULL keyEquivalent:@""];
	submenu = [[NSMenu alloc] initWithTitle:@"Apple"];
	[NSApp performSelector:@selector(setAppleMenu:) withObject:submenu];
	
	NSMenuItem * appMenuItem;
	
	appMenuItem = [submenu addItemWithTitle:NSLocalizedString(@"Preferences...", nil)  action:NULL keyEquivalent:@","];
	appMenuItem = [submenu addItemWithTitle:[NSString stringWithFormat:@"%@ %s", NSLocalizedString(@"Quit", nil),  (const char *)Ctrl::appname]
								action:@selector(terminate:)
						 keyEquivalent:@"q"];
	
//	[appMenuItem setTarget:NSApp];  //I don't know if we need to set targets here...
	[menuItem setTarget:NSApp];
	
	
	[mainMenu setSubmenu:submenu forItem:menuItem];
	
	
	/* Window part is tmp here. ok, for now it's just to see if our menu method loaded and works */
	NSMenuItem * winMenuItem;
	NSMenu * winsubmenu;
	winMenuItem = [mainMenu addItemWithTitle:@"Window" action:NULL keyEquivalent:@""];
	winsubmenu = [[NSMenu alloc] initWithTitle:NSLocalizedString(@"Window", @"Test Cocoa Window menu")];  //need to find upp way of localization 
	
	[mainMenu setSubmenu:winsubmenu forItem:winMenuItem];

	[NSApp setMainMenu:mainMenu];
	
}

void Ctrl::CocoaStart()
{
	Ctrl::cocoaStarted=true;
//	Ctrl::appname="TestCocoa1"; //too late in here

	NSLog(@"CocoaStart cocoaStarted=%s", (const char *)AsString(cocoaStarted));
	NSLog(@"CocoaStart GetTopCtrls().GetCount()=%d", GetTopCtrls().GetCount() );
//	NSLog(@"CocoaStart top->mwindowref=%@", GetWindowRef() );
	

//	NSLog(@"CocoaStart cocoaStarted=%s", (GetActiveWindow()));
	
	//GetTopCtrls();
	
//	Topt *tw=(this)->GetMainWindow();
	
//	Ctrl::EventLoop0();
	//if(*tw)
	//	tw->Run();
//	cocoaStarted=true;
}

void Ctrl::CocoaExit()
{
	cocoaTerminated=true;
	[NSApp setDelegate:nil];	
}

#endif

END_UPP_NAMESPACE