
#include "SystemDraw.h"
//#include "ConsoleDraw.h"

NAMESPACE_UPP

#ifdef PLATFORM_COCOA

extern String lllg;
#define LLOG(x)   lllg=""; \
lllg<<x; \
NSLog(@"%s",(const char*)lllg);


//void SystemDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
//{
//	CGContextDrawImage(context, CGRectMake(0, 0, width, height), imgRef); 
//}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
//	SetSurface(w.GetHandle(), dest, pixels, psz, poff);
	LLOG("DRAW IMAGE MAC SetSurface");
//	CGContextDrawImage(w.GetHandle(), CGRectMake(0, 0, psz.cx, psz.cy), (CGImage *) pixels);
	
}

int  Image::Data::GetResCountImp() const
{
	return 0; //!!Sys().picture + !!Sys().picture8;
}

ImageDraw::ImageDraw(int cx, int cy)
{
	LLOG("DRAW IMAGE MAC ImageDraw::ImageDraw(int cx, int cy) 1 "<<cx);	
	size = Size(cx, cy);
	Init();
	LLOG("DRAW IMAGE MAC ImageDraw::ImageDraw(int cx, int cy) 2 "<<cx);	

}

void ImageDraw::Section::Init(int cx, int cy)
{
	GuiLock __;
//	dc = ::CreateCompatibleDC(ScreenHDC());
//	dc = ScreenHDC();
//	CGFloat width = CGRectGetWidth(rect);
//    CGFloat height = CGRectGetHeight(rect);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    size_t bitsPerComponent = 8;
    size_t bytesPerPixel    = 4;
    size_t bytesPerRow      = (cx * bitsPerComponent * bytesPerPixel + 7) / 8;
    size_t dataSize         = bytesPerRow * cy;
    
 //   unsigned char *data = malloc(dataSize);
 //   memset(data, 0, dataSize);
	
    //CGContextRef context
		LLOG("DRAW IMAGE MAC ImageDraw::Section::Init !!!!!!!!!!! 1");
	dc= CGBitmapContextCreate((void **)&pixels, cx, cy, 
												 bitsPerComponent, 
												 bytesPerRow, colorSpace, 
												 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
	LLOG("DRAW IMAGE MAC ImageDraw::Section::Init !!!!!!!!!!! 2");
	
	
//	BitmapInfo32__ bi(cx, cy);
//	hbmp = CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
//	hbmpOld = (HBITMAP) ::SelectObject(dc, hbmp);
}

ImageDraw::Section::~Section()
{
	GuiLock __;
//	::DeleteObject(::SelectObject(dc, hbmpOld));
//	::DeleteDC(dc);
	LLOG("DRAW IMAGE MAC dc = NULL;");	
	
	dc = NULL; //?
}

void ImageDraw::Init()
{
	GuiLock __;
	LLOG("DRAW IMAGE MAC ImageDraw::Init() 1");	
	rgb.Init(size.cx, size.cy);
	a.Init(size.cx, size.cy);
	LLOG("DRAW IMAGE MAC ImageDraw::Init() 2");	
	
	Attach(rgb.dc);
	InitClip(size);
	alpha.Attach(a.dc);
	alpha.InitClip(size);
	LLOG("DRAW IMAGE MAC ImageDraw::Init() 3");	

	has_alpha = false;
}

Image Image::Wait() { return Null; }

ImageDraw::ImageDraw(Size sz)
{
	size = sz;
	Init();
}

ImageDraw::operator Image() const
{
	return Get(true);
}

ImageDraw::~ImageDraw()
{
	Detach();
	alpha.Detach();
}


void Image::Data::SysInitImp()
{
//	SystemData& sd = Sys();
//	sd.hbmp = sd.hmask = sd.himg = NULL;
//	sd.cursor_cheat = NULL;
}

void Image::Data::SysReleaseImp()
{
}

void Image::Data::PaintImp(SystemDraw& w, int x, int y, const Rect& src, Color c)
{
	LLOG("DRAW IMAGE MAC Image::Data::PaintImp !!!!!!!!!!!");	
	
}

Image ImageDraw::Get(bool pm) const
{
		LLOG("DRAW IMAGE MAC ImageDraw::Get(bool pm) !!!!!!!!!!!");
	return Null;
}

Image Image::Arrow() { return Null; }

#endif

END_UPP_NAMESPACE