#include "SystemDraw.h"

NAMESPACE_UPP

#ifdef PLATFORM_X11

struct Image::Data::SystemData {
	int         cursor_cheat;
	XPicture    picture;
	XPicture    picture8;
};

Image::Data::SystemData& Image::Data::Sys() const
{
	ASSERT(sizeof(system_buffer) >= sizeof(SystemData));
	return *(SystemData *)system_buffer;
}

void Image::SetCursorCheat(int id)
{
	data->Sys().cursor_cheat = id;
}

int Image::GetCursorCheat() const
{
	return data ? data->Sys().cursor_cheat : -1;
}

static void sInitXImage(XImage& ximg, Size sz)
{
	Zero(ximg);
	ximg.width            = sz.cx;
	ximg.height           = sz.cy;
	ximg.xoffset          = 0;
	ximg.format           = ZPixmap;
	ximg.bitmap_bit_order = MSBFirst;
#ifdef CPU_LITTLE_ENDIAN
	ximg.byte_order       = LSBFirst;
#else
	ximg.byte_order       = MSBFirst;
#endif
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size srcsz, Point srcoff)
{
	GuiLock __;
	XImage ximg;
	sInitXImage(ximg, srcsz);
	ximg.bitmap_pad = 32;
	ximg.bytes_per_line = sizeof(RGBA) * srcsz.cx;
	ximg.bits_per_pixel = 32;
	ximg.blue_mask = 0x00ff0000;
	ximg.green_mask = 0x0000ff00;
	ximg.red_mask = 0x000000ff;
	ximg.bitmap_unit = 32;
	ximg.depth = 24;
	ximg.data = (char *)pixels;
	XInitImage(&ximg);
	Drawable dw = w.GetDrawable();
	GC gc = XCreateGC(Xdisplay, dw, 0, 0);
	Point p = dest.TopLeft() + w.GetOffset();
	XPutImage(Xdisplay, dw, gc, &ximg, srcoff.x, srcoff.y,
	          p.x, p.y, dest.GetWidth(), dest.GetHeight());
	XFreeGC(Xdisplay, gc);
}

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	GuiLock __;
	Pixmap pixmap = XCreatePixmap(Xdisplay, Xroot, cx, cy, 24);
	XPicture picture = XRenderCreatePicture(
		Xdisplay, pixmap,
	    XRenderFindStandardFormat(Xdisplay, PictStandardRGB24),
	    0, 0
	);
	XImage ximg;
	sInitXImage(ximg, Size(cx, cy));
	ximg.bitmap_pad = 32;
	ximg.bytes_per_line = 4 * cx;
	ximg.bits_per_pixel = 32;
	ximg.blue_mask = 0x00ff0000;
	ximg.green_mask = 0x0000ff00;
	ximg.red_mask = 0x000000ff;
	ximg.bitmap_unit = 32;
	ximg.depth = 24;
	ximg.data = (char *)pixels;
	XInitImage(&ximg);
	GC gc = XCreateGC(Xdisplay, pixmap, 0, 0);
	XPutImage(Xdisplay, pixmap, gc, &ximg, 0, 0, 0, 0, cx, cy);
	XFreeGC(Xdisplay, gc);
	XFreePixmap(Xdisplay, pixmap);
	XRenderComposite(Xdisplay, PictOpOver,
	                 picture, 0, XftDrawPicture(w.GetXftDraw()),
	                 0, 0, 0, 0, x, y, cx, cy);
	XRenderFreePicture(Xdisplay, picture);
}

int  Image::Data::GetResCountImp() const
{
	return !!Sys().picture + !!Sys().picture8;
}

void Image::Data::SysInitImp()
{
	SystemData& sd = Sys();
	sd.picture = 0;
	sd.picture8 = 0;
	sd.cursor_cheat = -1;
}

void Image::Data::SysReleaseImp()
{
	SystemData& sd = Sys();
	if(sd.picture) {
		GuiLock __;
		if(Xdisplay) XRenderFreePicture(Xdisplay, sd.picture);
		ResCount -= !paintonly;
		sd.picture = 0;
	}
	if(sd.picture8) {
		GuiLock __;
		if(Xdisplay) XRenderFreePicture(Xdisplay, sd.picture8);
		ResCount -= !paintonly;
		sd.picture8 = 0;
	}
}

struct XRSolidFill {
	Color    color;
	XPicture picture;
	Pixmap   pixmap;
};

enum { XRSolidFillCount = 67 };

static XRSolidFill sFill[XRSolidFillCount];

inline int s255d16(int x)
{
	return (x << 8)|x;
}

static XPicture sGetSolidFill(Color c)
{
	GuiLock __;
	int q = GetHashValue(c) % (int)XRSolidFillCount;
	XRSolidFill& f = sFill[q];
	if(f.color == c && f.picture)
		return f.picture;
	if(f.picture)
		XRenderFreePicture(Xdisplay, f.picture);
	if(f.pixmap)
		XFreePixmap(Xdisplay, f.pixmap);
	f.pixmap = XCreatePixmap(Xdisplay, Xroot, 1, 1, 32);
	XRenderPictureAttributes attr;
	attr.repeat = XTrue;
	f.picture = XRenderCreatePicture(Xdisplay, f.pixmap,
	                                 XRenderFindStandardFormat(Xdisplay, PictStandardARGB32),
	                                                           CPRepeat, &attr);
	f.color = c;
	XRenderColor xc;
	xc.red = s255d16(c.GetR());
	xc.green = s255d16(c.GetG());
	xc.blue = s255d16(c.GetB());
	xc.alpha = 65535;
	XRenderFillRectangle(Xdisplay, PictOpSrc, f.picture, &xc, 0, 0, 1, 1);
	return f.picture;
}

void Image::Data::PaintImp(SystemDraw& w, int x, int y, const Rect& src, Color c)
{
	GuiLock __;
	SystemData& sd = Sys();
	while(ResCount > 512) {
		Image::Data *l = ResData->GetPrev();
		l->SysRelease();
		l->Unlink();
	}
	x += w.GetOffset().x;
	y += w.GetOffset().y;
	Size sz = buffer.GetSize();
	int  len = sz.cx * sz.cy;
	Rect sr = src & sz;
	Size ssz = sr.Size();
	if(sr.IsEmpty())
		return;
	if(GetKind() == IMAGE_EMPTY)
		return;
	if(GetKind() == IMAGE_OPAQUE && !IsNull(c)) {
		w.DrawRect(x, y, sz.cx, sz.cy, c);
		return;
	}
	if(GetKind() == IMAGE_OPAQUE && paintcount == 0 && sr == Rect(sz)) {
		SetSurface(w, x, y, sz.cx, sz.cy, buffer);
		paintcount++;
		return;
	}
	Unlink();
	LinkAfter(ResData);
	if(IsNull(c)) {
		if(!sd.picture) {
			bool opaque = GetKind() == IMAGE_OPAQUE;
			Pixmap pixmap = XCreatePixmap(Xdisplay, Xroot, sz.cx, sz.cy, opaque ? 24 : 32);
			sd.picture = XRenderCreatePicture(
				Xdisplay, pixmap,
			    XRenderFindStandardFormat(Xdisplay, opaque ? PictStandardRGB24
			                                               : PictStandardARGB32),
			    0, 0
			);
			ResCount++;
			XImage ximg;
			sInitXImage(ximg, sz);
			ximg.bitmap_pad = 32;
			ximg.bytes_per_line = 4 * sz.cx;
			ximg.bits_per_pixel = 32;
			ximg.blue_mask = 0x00ff0000;
			ximg.green_mask = 0x0000ff00;
			ximg.red_mask = 0x000000ff;
			ximg.bitmap_unit = 32;
			ximg.data = (char *)~buffer;
			ximg.depth = opaque ? 24 : 32;
			XInitImage(&ximg);
			GC gc = XCreateGC(Xdisplay, pixmap, 0, 0);
			XPutImage(Xdisplay, pixmap, gc, &ximg, 0, 0, 0, 0, sz.cx, sz.cy);
			XFreeGC(Xdisplay, gc);
			XFreePixmap(Xdisplay, pixmap);
			PaintOnlyShrink();
		}
		XRenderComposite(Xdisplay, PictOpOver,
		                 sd.picture, 0, XftDrawPicture(w.GetXftDraw()),
		                 sr.left, sr.top, 0, 0, x, y, ssz.cx, ssz.cy);
	}
	else {
		ASSERT(!paintonly);
		if(!sd.picture8) {
			Pixmap pixmap = XCreatePixmap(Xdisplay, Xroot, sz.cx, sz.cy, 8);
			sd.picture8 = XRenderCreatePicture(Xdisplay, pixmap,
			                                   XRenderFindStandardFormat(Xdisplay, PictStandardA8),
			                                   0, 0);
			ResCount++;
			Buffer<byte> ab(len);
			byte *t = ab;
			const RGBA *s = buffer;
			const RGBA *e = s + len;
			while(s < e)
				*t++ = (s++)->a;
			XImage ximg;
			sInitXImage(ximg, sz);
			ximg.data             = (char *)~ab;
			ximg.bitmap_unit      = 8;
			ximg.bitmap_pad       = 8;
			ximg.depth            = 8;
			ximg.bytes_per_line   = sz.cx;
			ximg.bits_per_pixel   = 8;
			XInitImage(&ximg);
			GC gc = XCreateGC(Xdisplay, pixmap, 0, 0);
			XPutImage(Xdisplay, pixmap, gc, &ximg, 0, 0, 0, 0, sz.cx, sz.cy);
			XFreeGC(Xdisplay, gc);
			XFreePixmap(Xdisplay, pixmap);
		}
		XRenderComposite(Xdisplay, PictOpOver,
		                 sGetSolidFill(c), sd.picture8, XftDrawPicture(w.GetXftDraw()),
		                 sr.left, sr.top, 0, 0, x, y, ssz.cx, ssz.cy);
	}
}

void ImageDraw::Init()
{
	GuiLock __;
	dw = XCreatePixmap(Xdisplay, Xroot, max(size.cx, 1), max(size.cy, 1), Xdepth);
	gc = XCreateGC(Xdisplay, dw, 0, 0);
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw, DefaultVisual(Xdisplay, Xscreenno), Xcolormap);

	alpha.dw = XCreatePixmap(Xdisplay, Xroot, max(size.cx, 1), max(size.cy, 1), Xdepth);
	alpha.gc = XCreateGC(Xdisplay, alpha.dw, 0, 0);
	alpha.xftdraw = XftDrawCreate(Xdisplay, (Drawable) alpha.dw, DefaultVisual(Xdisplay, Xscreenno), Xcolormap);

	Vector<Rect> clip;
	clip.Add(RectC(0, 0, size.cx, size.cy));
	SystemDraw::Init(clip, Point(0, 0));
	alpha.Init(clip, Point(0, 0));

	has_alpha = false;
}

ImageDraw::operator Image() const
{
	GuiLock __;
	XImage *xim = XGetImage(Xdisplay, dw, 0, 0, max(size.cx, 1), max(size.cy, 1), AllPlanes, ZPixmap);
	Visual *v = DefaultVisual(Xdisplay, Xscreenno);
	RasterFormat fmt;

	RGBA   palette[256];

	switch(xim->depth) {
	case 15:
	case 16:
		if(xim->byte_order == LSBFirst)
			fmt.Set16le(v->red_mask, v->green_mask, v->blue_mask);
		else
			fmt.Set16be(v->red_mask, v->green_mask, v->blue_mask);
		break;
	case 8: {
		int n = min(v->map_entries, 256);
		XColor colors[256];
		for(int i = 0; i < 256; i++) {
			colors[i].pixel = i;
			colors[i].flags = DoRed|DoGreen|DoBlue;
		}
		XQueryColors(Xdisplay, Xcolormap, colors, n);
		XColor *s = colors;
		XColor *e = s + n;
		while(s < e) {
			RGBA& t = palette[s->pixel];
			t.r = s->red >> 8;
			t.g = s->green >> 8;
			t.b = s->blue >> 8;
			t.a = 255;
			s++;
		}
		fmt.Set8();
		break;
	}
	default:
		if(xim->bits_per_pixel == 32)
			if(xim->byte_order == LSBFirst)
				fmt.Set32le(v->red_mask, v->green_mask, v->blue_mask);
			else
				fmt.Set32be(v->red_mask, v->green_mask, v->blue_mask);
		else
			if(xim->byte_order == LSBFirst)
				fmt.Set24le(v->red_mask, v->green_mask, v->blue_mask);
			else
				fmt.Set24be(v->red_mask, v->green_mask, v->blue_mask);
		break;
	}

	ImageBuffer ib(size);
	const byte *s = (const byte *)xim->data;
	RGBA *t = ib;
	for(int y = 0; y < size.cy; y++) {
		fmt.Read(t, s, size.cx, palette);
		s += xim->bytes_per_line;
		t += size.cx;
	}
	XDestroyImage(xim);
	if(has_alpha) {
		xim = XGetImage(Xdisplay, alpha.dw, 0, 0, size.cx, size.cy, AllPlanes, ZPixmap);
		const byte *s = (const byte *)xim->data;
		t = ib;
		Buffer<RGBA> line(size.cx);
		for(int y = 0; y < size.cy; y++) {
			fmt.Read(line, s, size.cx, palette);
			for(int x = 0; x < size.cx; x++)
				(t++)->a = line[x].r;
			s += xim->bytes_per_line;
		}
		XDestroyImage(xim);
	}
	Premultiply(ib);
	return ib;
}

ImageDraw::ImageDraw(Size sz)
{
	size = sz;
	Init();
}

ImageDraw::ImageDraw(int cx, int cy)
{
	size = Size(cx, cy);
	Init();
}

ImageDraw::~ImageDraw()
{
	GuiLock __;
	XftDrawDestroy(xftdraw);
	XFreePixmap(Xdisplay, dw);
	XFreeGC(Xdisplay, gc);
	XftDrawDestroy(alpha.xftdraw);
	XFreePixmap(Xdisplay, alpha.dw);
	XFreeGC(Xdisplay, alpha.gc);
}

Image sX11Cursor__(int c)
{
	ImageBuffer b(32, 32);
	Image m(b);
	m.SetCursorCheat(c);
	return m;
}

#include <X11/cursorfont.h>

#define FCURSOR_(x) { Image h; INTERLOCKED { static Image m = sX11Cursor__(x); h = m; } return h; }

Image Image::Arrow() FCURSOR_(XC_left_ptr)
Image Image::Wait() FCURSOR_(XC_watch)
Image Image::IBeam() FCURSOR_(XC_xterm)
Image Image::No() FCURSOR_(XC_circle)
Image Image::SizeAll() FCURSOR_(XC_fleur)
Image Image::SizeHorz() FCURSOR_(XC_sb_h_double_arrow)
Image Image::SizeVert() FCURSOR_(XC_double_arrow)
Image Image::SizeTopLeft() FCURSOR_(XC_top_left_corner)
Image Image::SizeTop() FCURSOR_(XC_top_side)
Image Image::SizeTopRight() FCURSOR_(XC_top_right_corner)
Image Image::SizeLeft() FCURSOR_(XC_left_side)
Image Image::SizeRight() FCURSOR_(XC_right_side)
Image Image::SizeBottomLeft() FCURSOR_(XC_bottom_left_corner)
Image Image::SizeBottom() FCURSOR_(XC_bottom_side)
Image Image::SizeBottomRight()  FCURSOR_(XC_bottom_right_corner)
Image Image::Cross() FCURSOR_(XC_crosshair)
Image Image::Hand() FCURSOR_(XC_hand1)

void *X11Cursor(const Image& img)
{
	GuiLock __;
	int q = img.GetCursorCheat();
	if(q >= 0)
		return (void *)XCreateFontCursor(Xdisplay, q);
	int len = img.GetLength();
	Size sz = img.GetSize();
	Pixmap pixmap = XCreatePixmap(Xdisplay, Xroot, sz.cx, sz.cy, 32);
	XPicture picture = XRenderCreatePicture(
		Xdisplay, pixmap,
	    XRenderFindStandardFormat(Xdisplay, PictStandardARGB32),
	    0, 0
	);
	XImage ximg;
	sInitXImage(ximg, sz);
	ximg.bitmap_pad = 32;
	ximg.bytes_per_line = 4 * sz.cx;
	ximg.bits_per_pixel = 32;
	ximg.blue_mask = 0x00ff0000;
	ximg.green_mask = 0x0000ff00;
	ximg.red_mask = 0x000000ff;
	Buffer<RGBA> pma;
	pma.Alloc(len);
	memcpy(pma, ~img, len * sizeof(RGBA));
	ximg.bitmap_unit = 32;
	ximg.depth = 32;
	ximg.data = (char *)~pma;
	XInitImage(&ximg);
	GC gc = XCreateGC(Xdisplay, pixmap, 0, 0);
	XPutImage(Xdisplay, pixmap, gc, &ximg, 0, 0, 0, 0, sz.cx, sz.cy);
	XFreeGC(Xdisplay, gc);
	XFreePixmap(Xdisplay, pixmap);
	Point p = img.GetHotSpot();
	Cursor c = XRenderCreateCursor(Xdisplay, picture, p.x, p.y);
	XRenderFreePicture(Xdisplay, picture);
	return (void *)c;
}

#endif

END_UPP_NAMESPACE
