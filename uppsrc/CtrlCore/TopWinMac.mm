#include "CtrlCore.h"
//#import <Cocoa/Cocoa.h>
//#include "SystemDraw.h"
//#include "ConsoleDraw.h"

NAMESPACE_UPP

#ifdef  PLATFORM_COCOA

extern String lllg;
#define LLOG(x)   lllg=""; \
lllg<<x; \
NSLog(@"%s",(const char*)lllg);


void    TopWindow::SyncSizeHints() {}

void TopWindow::SyncCaption0()
{
}

void TopWindow::Maximize(bool effect)
{
}


bool Ctrl::IsWndOpen() const {
	GuiLock __;
	return GetWindowRef();
//	return true;  //tmp
}

void TopWindow::SyncTitle0()
{
}

void TopWindow::Open(WindowRef hwnd)
{
	GuiLock __;
//	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
//		DistributeAccessKeys();
//	UsrLogT(3, "OPEN " + Desc(this));
//	LLOG("TopWindow::Open, owner HWND = " << FormatIntHex((int)hwnd, 8) << ", Active = " << FormatIntHex((int)::GetActiveWindow(), 8));
//	IgnoreMouseUp();
//	SyncCaption();

//	if(fullscreen) {
//			SetRect(GetScreenSize());
//			Create(hwnd, WS_POPUP, 0, false, SW_SHOWMAXIMIZED, false);
//		}
//		else {
//			CenterRect(hwnd, hwnd && hwnd == GetTrayHWND__() ? center ? 2 : 0 : center);
//			Create(hwnd, style, exstyle, false, state == OVERLAPPED ? SW_SHOWNORMAL :
//				   state == MINIMIZED  ? SW_MINIMIZE :
//				   SW_MAXIMIZE, false);
	NSLog(@"TopWindow::Open b4 Create");

	Create(hwnd, style, exstyle, false, 0, false);
	NSLog(@"TopWindow::Open after Create");
	
//		}
//	PlaceFocus();
//	SyncCaption();
//	FixIcons();
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	LLOG("TopWindow::Open(Ctrl *owmer) -> " << UPP::Name(owner));
	Open(owner ? owner->GetTopCtrl()->GetWindowRef() : NULL);
	if(IsOpen() && top)
		top->owner = owner;
}

void TopWindow::Open()
{
	LLOG("TopWindow::Open b4 GetActiveWindow");
	Open(GetActiveWindow());
}

void TopWindow::OpenMain()
{
	Open((WindowRef) NULL);
}

#endif

END_UPP_NAMESPACE