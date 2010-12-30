#include "CtrlCore.h"
//#include "ConsoleDraw.h"

NAMESPACE_UPP

#ifdef  PLATFORM_COCOA

const char *ClipFmtsImage()
{
	static const char *q;
	return q;
}

String GetImageClip(const Image& img, const String& fmt)
{
	return Null;
}

bool AcceptImage(PasteClip& clip)
{
	GuiLock __;
	return clip.Accept(ClipFmtsImage());
}

Image GetImage(PasteClip& clip)
{
	return Null;
}


#endif

END_UPP_NAMESPACE
