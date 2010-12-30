#include "CtrlCore.h"
//#include "SystemDraw.h"
//#include "ConsoleDraw.h"
#import <Cocoa/Cocoa.h>

NAMESPACE_UPP

#ifdef  PLATFORM_COCOA

bool GetShift() { GuiLock __; return false; }
bool GetCtrl() { GuiLock __; return false; }
bool GetAlt() { GuiLock __; return false; }
bool GetCapsLock() { GuiLock __; return false; }
bool GetMouseLeft() { GuiLock __; return false; }
bool GetMouseRight() { GuiLock __; return false; }
bool GetMouseMiddle() { GuiLock __; return false; }


#endif

END_UPP_NAMESPACE