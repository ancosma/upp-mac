#ifdef PLATFORM_COCOA


#include <Carbon/Carbon.h>


NAMESPACE_UPP

//#undef Ptr;
//#undef Size;




//#define HDC CGContextRef;

class SystemDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Size  GetPageSize() const;
	
	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;
	virtual Rect GetPaintRect() const;
	
	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);
	
	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);
	
	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	
	virtual Size GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();
	
	virtual int  GetCloffLevel() const;
	
private:
	Size  pageSize;
	Size  nativeSize;
	Size  nativeDpi;
	bool  palette:1;
	bool  color16:1;
	bool  is_mono:1;
	int   native;
	
	friend class  ImageDraw;
	friend class  FontInfo;
	friend class  Font;
	
	friend void StaticExitDraw_();
	
	Point     actual_offset_bak;
	
	struct Cloff : Moveable<Cloff> {
		Point org;
		//		HRGN  hrgn;
		Rect  drawingclip;
	};
	
	Array<Cloff> cloff;
	Rect         drawingclip;
	
	//	COLORREF  lastTextColor;
	Color     lastColor;
	//	HBRUSH    orgBrush;
	//	HBRUSH    actBrush;
	//	HPEN      orgPen;
	//	HPEN      actPen;
	int       lastPen;
	Color     lastPenColor;
	
	void   Unselect0();
	void   Cinit();
	
	void   LoadCaps();
	void   SetPrinterMode();
	void   Reset();
	void   SetOrg();
	//	friend HPALETTE GetQlibPalette();
	void   DotsMode();
	
	static void      InitColors();
	
	friend class BackDraw;
	friend class ScreenDraw;
	friend class PrintDraw;
	
protected:
	dword style;
	CGContextRef   handle;
	Point actual_offset;
	
	SystemDraw();
	void   Init();
	void   InitClip(const Rect& clip);
	
public:
	static Rect GetVirtualScreenArea();
	
	static void SetAutoPalette(bool ap);
	static bool AutoPalette();
	bool PaletteMode()                                  { return palette; }
	
	static void Flush()                                 { } //GdiFlush(); }
	
	//	COLORREF GetColor(Color color) const;
	
	Point    GetOffset() const                          { return actual_offset; }
	
#ifndef PLATFORM_WINCE
	//	Point LPtoDP(Point p) const;
	//	Point DPtoLP(Point p) const;
	//	Rect  LPtoDP(const Rect& r) const;
	//	Rect  DPtoLP(const Rect& r) const;
#endif
	
	void SetColor(Color color);
	void SetDrawPen(int width, Color color);
	
	Size  GetSizeCaps(int i, int j) const;
	CGContextRef BeginGdi();
	void  EndGdi();
	CGContextRef   GetHandle()                    { return handle; }
	operator CGContextRef() const                 { return handle; }
	void  Unselect();
	void  Attach(CGContextRef ahandle)            { handle = ahandle; Init(); }
	CGContextRef   Detach()                       { Unselect(); CGContextRef h = handle; handle = NULL; return h; }
	
	SystemDraw(CGContextRef hdc);
	
	virtual ~SystemDraw();
};


class ScreenDraw : public SystemDraw {
public:
	ScreenDraw(bool ic = false);
	~ScreenDraw();
};

//CGContextRef      ScreenHDC();

END_UPP_NAMESPACE

#endif
