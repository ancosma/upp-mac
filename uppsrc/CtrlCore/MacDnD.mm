#include "CtrlCore.h"
//#include "ConsoleDraw.h"

NAMESPACE_UPP


#ifdef  PLATFORM_COCOA


int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	return 0;
}

#endif

END_UPP_NAMESPACE
