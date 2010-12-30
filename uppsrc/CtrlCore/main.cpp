#include <CtrlCore/CtrlCore.h>
//#include "CtrlCore_Prefix.h"


using namespace Upp;

#define LLOG(x)   


//String lllg1;
//#define LLOG(x)   lllg1=""; \
//lllg1<<x; \
//NSLog(@"%s",(const char*)lllg1);



struct App : TopWindow {
//	Image img;
	
	virtual void Paint(Draw& w) {
		LLOG("Test draw from maim virtual void Paint(Draw& w)");
//		w.DrawRect(GetSize(), SWhite);
		UPP::Size sz = GetSize();
		GetWorkArea();
//		w.DrawRect(0, 0, sz.cx, sz.cy, LtGreen());
		w.DrawRect(10, 10, 30, 30, SRed);
		w.DrawLine(25, 25, 80, 120, 4, SBlue);
//		w.DrawLine(80, 90, 400, 0, PEN_DASHDOT);
//		w.DrawEllipse(200, 200, 50, 100, Green);
	//	w.DrawImage(0, 0, img);

		
	}
	
	App() {
		LLOG("Test draw from maim ImageDraw w(100, 100); 1");

	//	ImageDraw w(150, 100);
		LLOG("Test draw from maim ImageDraw w(100, 100); 2");
	//	w.DrawRect(0, 0, 100, 100, SBlack);
	//	LLOG("Test draw from maim w.DrawRect(0, 0, 100, 100, SBlack); ");
		
//		w.DrawEllipse(0, 0, 100, 100, SGreen);
//		w.DrawText(0, 0, "ImageDraw!", Arial(13).Bold(), SYellow);
//		img = w;
	//	Sizeable();
	}
};

GUI_APP_MAIN
{
	LLOG("Test draw from maim App().Run();");	
	App().Run();
}
