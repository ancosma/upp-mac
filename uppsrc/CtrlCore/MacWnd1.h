#ifndef __MACWND1_H__
#define __MACWND1_H__

//#ifdef __cplusplus
//extern "C" {
//#endif


#include "CtrlCore.h"


namespace Upp
{

	class UppCocoaWindow {  //maybe we need to have our own upp cocoa window implementation?
	private:
		UNSWindow * window_;
		UNSView * view_;
	public:
		
		//	protected:
		UppCocoaWindow() {}
		virtual ~UppCocoaWindow() {}
		
	};
	
}


#endif