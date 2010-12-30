
#include "CtrlCore.h"

NAMESPACE_UPP

#ifdef  PLATFORM_COCOA
extern String lllg;
#define LLOG(x) {  lllg=""; \
lllg<<x; \
NSLog(@"%s",(const char*)lllg); }

template<>
unsigned GetHashValue(const WindowRef& h)
{
	return (unsigned)(intptr_t)h;
}

VectorMap< WindowRef, Ptr<Ctrl> >& Ctrl::Windows()
{
	static VectorMap< WindowRef, Ptr<Ctrl> > map;
	LLOG("VectorMap< WindowRef count=" << map.GetCount() );

	return map;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> v;
	VectorMap< WindowRef, Ptr<Ctrl> >& w = Windows();
	LLOG("GetTopCtrls() A!!! ="<<w.GetCount());	
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i] && !w[i]->parent)
			v.Add(w[i]);
	LLOG("GetTopCtrls() B!!! ="<<w.GetCount());	
	return v;
}

Ctrl *Ctrl::CtrlFromWindowRef(WindowRef _windowref)
{
	GuiLock __;
//	NSLog(@"////CtrlFromWindowRef()////  = %@", (void *)Windows().Get(_windowref, NULL) );
	return _windowref ? Windows().Get(_windowref, NULL) : NULL;
}

WindowRef Ctrl::GetOwnerWindowRef() const
{
	GuiLock __;
	WindowRef hwnd = GetWindowRef();
//	NSLog(@"GetOwnerWindowRef() A!!! = %@", hwnd);	
	if(!hwnd) return NULL;
//	NSLog(@"GetOwnerWindowRef() B!!! = %@", hwnd);
//	return GetWindow(hwnd, GW_OWNER);  //do we have owners in cocoa?
	return (WindowRef)[(NSWindow *)hwnd parentWindow];
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	LLOG("::GetOwner");
	WindowRef hwnd = GetOwnerWindowRef();
//	NSLog(@"*Ctrl::GetOwner()  GetOwnerWindowRef() !!! = %@", hwnd);
//	NSLog(@"*Ctrl::GetOwner()  CtrlFromWindowRef(hwnd) !!! = %@", CtrlFromWindowRef(hwnd) );	
	return hwnd ? CtrlFromWindowRef(hwnd) : NULL;
}

Rect Ctrl::GetDefaultWindowRect() {
	return Rect(0, 0, 200, 200);
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return focusCtrl ? focusCtrl->GetTopCtrl() : NULL;
}

void Ctrl::WndCreateCaret0(const Rect& cr)
{
}
void Ctrl::WndCreateCaret(const Rect& cr)
{
}


/* Ctrl::GetWorkArea() Returns OS specific working area for the widget
 - this is a rectangle in screen coordinates where 
 application top-level windows can be placed
 - the size of screen
 minus
 the size of any border entities
	like the task-bar.
 If widget is not associated with the open window,
 returns the size of primary work area.
 */

Rect Ctrl::GetWorkArea() const  //FIXME
{
	NSWindow * nswin = (NSWindow*)GetWindowRef();
	Rect r = RectC(0,0,0,0);
	if (nswin != NULL) {  //assert?
		NSRect sr = [nswin frame];
		r = RectC(sr.origin.x, sr.origin.y, sr.size.width, sr.size.height);
	}
	LLOG("Ctrl::GetWorkArea()="<<r);
	return r;
	
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
//	MultiMon().EnumDisplayMonitors(NULL, NULL, &sMonEnumProc, (LPARAM)&rc);
	
}

Rect Ctrl::GetWorkArea(Point pt)
{
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

/* Ctrl::GetVirtualWorkArea() returns 
 the total size of all displays
 minus
 any border entities.
 */
Rect Ctrl::GetVirtualWorkArea()
{
	Rect out = GetPrimaryWorkArea();
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		out |= rc[i];
	return out;
}


/* Ctrl::GetVirtualScreenArea() returns
the total size of all displays
*/
Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	//FIXME loop for all screens?
	return GetPrimaryScreenArea();

}

/* Ctrl::GetPrimaryWorkArea() returns
 the size of primary work area
 - primary screen in Windows is the display with start menu?
 ? minus Mac menubar & minus dock?
*/
 Rect Ctrl::GetPrimaryWorkArea()
{
	Rect r;
	NSRect  sr =[[NSScreen mainScreen] visibleFrame];
	//btw, mainScreen contains the window with the keyboard focus!
	
	r = RectC(0, 0 , sr.size.width, sr.size.height);
	//where top 0 should start? below the mac menubar?
	
//	LLOG("Ctrl::GetPrimaryWorkArea = " << r);	
	return r;
}

/* Ctrl::GetPrimaryScreenArea() returns
 the size of primary screen
 - primary screen in Windows is the display with start menu
 */
Rect Ctrl::GetPrimaryScreenArea()
{
	Rect r;
	NSRect  sr =[[NSScreen mainScreen] frame];
	
	r = RectC(0, 0 , sr.size.width, sr.size.height);
	
	//	LLOG("Ctrl::GetPrimaryScreenArea = " << r);	
	return r;
	
}

int Ctrl::GetKbdDelay()
{
	GuiLock __;
	return 500;
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
	return 0;
}

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
//	LLOG("Ctrl::WndEnable(" << b << ") in " << UPP::Name(this) << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	if(*b)
		ReleaseCapture();
//	LLOG("//Ctrl::WndEnable(" << b << ") -> false " <<UPP::Name(this) << ", focusCtrlWnd = " <<UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
}

bool Ctrl::IsWndForeground() const
{
	return false;
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
}

void Ctrl::WndShow0(bool b)
{
}

void Ctrl::WndUpdate0()
{
}

void Ctrl::SetWndForeground0()
{
}

void Ctrl::WndUpdate0r(const Rect& r)
{
}

void Ctrl::WndDestroyCaret()
{
}

bool Ctrl::HasWndFocus() const
{
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	return false;
}

bool Ctrl::ProcessEvent(bool *quit)
{
//	LLOG("Ctrl::ProcessEvent(bool *quit)");
//	ASSERT(IsMainThread());
//	if(DoCall())
//		return false;
//	if(EndSession())
//		return false;
//	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
//		ReleaseCtrlCapture();
//	MSG msg;
//	if(PeekMsg(msg)) {
//		if(msg.message == WM_QUIT && quit)
//			*quit = true;
		//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": sProcessMSG " << FormatIntHex(msg.message));
//		sProcessMSG(msg);
		//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //sProcessMSG " << FormatIntHex(msg.message));
//		DefferedFocusSync();
//		SyncCaret();
//		return true;
//	}
	return false;
	
}


void Ctrl::EventLoop0(Ctrl *ctrl)
{
	LLOG("Ctrl::EventLoop0 1");
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LLOG("Ctrl::EventLoop0 2" << (void *)ctrl);
	LoopLevel++;
//	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
		
	}
	LLOG("ctrl->inloop ="<<ctrl->inloop);
	
	bool quit = false;
	ProcessEvents(&quit);
//	while(!EndSession() && !quit && ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())
	while(!cocoaTerminated && !quit && ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())

	{
		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
//		SyncCaret();
//		LLOG("ctrl->inloop ="<<ctrl->inloop);
	
//		GuiSleep(1000);
	//	if(EndSession()) break;
//				LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//				LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}
	
	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
//	LLOG(LOG_END << "Leaving event loop ");	
}

struct Ctrl::CreateBox {
	WindowRef  parent;
	DWORD style;
	DWORD exstyle;
	bool  savebits;
	int   show;
	bool  dropshadow;
};

void Ctrl::Create(WindowRef parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow)
{
	NSLog(@"Ctrl::Create(WindowRef 1");

	CreateBox cr;
	cr.parent = parent;
	cr.style = style;
	cr.exstyle = exstyle;
	cr.savebits = savebits;
	cr.show = show;
	cr.dropshadow = dropshadow;
	ICall(callback1(this, &Ctrl::Create0, &cr));
	NSLog(@"Ctrl::Create(WindowRef 2");

}

void Ctrl::Create0(Ctrl::CreateBox *cr)
{
	GuiLock __;
//	ASSERT(IsMainThread());
//	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in " <<UPP::Name(this) << BeginIndent);
	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in ");
	// <<UPP::Name(this));// << BeginIndent);
	
//	ASSERT(!IsChild() && !IsOpen());
//	Rect r = GetRect();
//	AdjustWindowRectEx(r, cr->style, FALSE, cr->exstyle);
	isopen = true;
//	NSLog(@"Ctrl::Create0 b4 new Top");

	top = new Top;
//	NSLog(@"Ctrl::Create0 after new Top");

//	ASSERT(!cr->parent || IsWindow(cr->parent));
//	ASSERT(!cr->parent);
	
//	if(!IsWinXP())
		NSAutoreleasePool *pool1 = [[NSAutoreleasePool alloc] init];
//	UppMacNSApplication *app =(UppMacNSApplication *)[UppMacNSApplication sharedApplication];
//	NSLog(@"app created");
	NSString* frameAsString = NSStringFromRect([[NSScreen mainScreen] visibleFrame]);
	NSLog(@"mainScreen frame = %@", frameAsString);

	//		NSScreen* mainScreen = [[NSScreen screens] objectAtIndex:0];
	NSRect topframe = NSMakeRect(100.0f, 250.0f, 500.0f, 250.0f);
	rect =RectC(100, 250, 500, 250);
	NSUInteger styleMask =  NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask; //  NSBorderlessWindowMask;
	
	NSRect windowRect = [NSWindow contentRectForFrameRect:topframe styleMask:styleMask];
	
		NSLog(@"mainScreen frame 2");
	
	//	self = [super initWithContentRect:[mainScreen frame]
	UNSWindow *window  =  [[UNSWindow alloc]
							 //	UppNSWindow  window =  [UppNSWindow
							 initWithContentRect:windowRect
							 styleMask:styleMask
							 backing:NSBackingStoreBuffered
							 defer:NO
							screen:nil];
	NSLog(@"mainScreen frame 3");

	//	[UppNSWindow initialize];
//	UNSApplicationDelegate.window=window;
	[window makeKeyAndOrderFront: NSApp];
	[window makeFirstResponder: window];	
	cr->dropshadow = false;

//	top->hwnd = CreateWindowEx(cr->exstyle,
//		                           cr->savebits ? cr->dropshadow ? "UPP-CLASS-SB-DS-A" : "UPP-CLASS-SB-A"
//								   : cr->dropshadow ? "UPP-CLASS-DS-A"    : "UPP-CLASS-A",
//		                           "", cr->style, r.left, r.top, r.Width(), r.Height(),
//		                           cr->parent, NULL, hInstance, this);
	
	inloop = false;
	
//	ASSERT(top->mwindowref);
	
//	::ShowWindow(top->hwnd, visible ? cr->show : SW_HIDE);
	[window setTitle:@"Test mac TopWin"];
	UNSView *view;
    view = [[UNSView new] initWithFrame:topframe ];
//	[view 
	[window setContentView:view];
	[window setOwnerCtrl:(Ctrl*)this];
	[view setOwnerCtrl:(Ctrl*)this];
	
//	[[window graphicsContext] graphicsPort];  //check this!!
	
//	[window setIgnoresMouseEvents: YES];
//	[window setDelegate: window];
//	OSStatus error1;
	
//	error1 = InstallStandardEventHandler(GetWindowEventTarget(window));
//	[window setInitialFirstResponder:window];

//	ShowWindow(window);  //should work in Carbon?
//    [window makeMainWindow: window];  ///!!!!!

	top->mwindowref = (WindowRef)window;  //where does this go?
	
	
	
  	
//	windowref =(WindowRef)window;
//	Windows();
	//it's probably not correct place to do this - need to find out how creating NSWindow can be cought in NSApp Event
	Windows().Add((WindowRef)window) = this;
	[pool1 release];
	//	::UpdateWindow(hwnd);
	StateH(OPEN);
	NSLog(@"Ctrl::Create0 STATEH(Open) %@", top->mwindowref);
	
//	LLOG(EndIndent << "Ctrl::Create in " <<UPP::Name(this));
//	LLOG("//Ctrl::Create in " <<UPP::Name(this));
	
//	RegisterDragDrop(top->hwnd, (LPDROPTARGET) (top->dndtgt = NewUDropTarget(this)));
	CancelMode();
	RefreshLayoutDeep();
}



bool Ctrl::IsCompositedGui()
{
	return false;
}

bool Ctrl::HasWndCapture() const
{
	return false;	
}

bool Ctrl::SetWndCapture()
{
	return false;
}



ViewDraw::~ViewDraw()
{
}

void  Ctrl::SetMouseCursor(const Image& image)
{
}

void Ctrl::WndInvalidateRect0(const Rect& r)
{
}

bool Ctrl::ProcessEvents(bool *quit)
{
//	LLOG("Ctrl::ProcessEvents");
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
//		LLOG("while(Ctrl::ProcessEvents SweepMkImageCache()");
		//		SweepMkImageCache();
		return true;
	}
//	LLOG("Ctrl::ProcessEvents return")
//	SweepMkImageCache();  //create!!
	return false;
}

void Ctrl::WndDestroy0()
{
}

void Ctrl::WndSetPos0(const Rect& rect)
{
}

ViewDraw::ViewDraw(Ctrl *ctrl)
{
}

void Ctrl::SetWndFocus0(bool *b)
{
}

Point GetMousePos() {
	Point p;
//	return ::GetCursorPos(p) ? p : Null;
//	::GetCursorPos(p);
	return p;
}
#endif

END_UPP_NAMESPACE

//#endif