class Display {
public:
	enum {
		CURSOR   = 0x01,
		FOCUS    = 0x02,
		SELECT   = 0x04,
		READONLY = 0x08,
	};

	virtual void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                             Color ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;
#ifdef flagSO
	Display();
#endif
	virtual ~Display();
};

struct AttrText {
	WString text;
	Font    font;
	Color   ink;
	Color   normalink;
	Color   paper;
	int     align;
	Image   img;
	int     imgspc;

	AttrText& Set(const char *s)                    { text = s; return *this; }
	AttrText& Set(const wchar *s)                   { text = s; return *this; }
	AttrText& Set(const WString& s)                 { text = s; return *this; }
	AttrText& operator=(const char *s)              { text = s; return *this; }
	AttrText& operator=(const wchar *s)             { text = s; return *this; }
	AttrText& operator=(const WString& s)           { text = s; return *this; }
	AttrText& operator=(const String& s)            { text = s.ToWString(); return *this; }
	AttrText& Ink(Color c)                          { ink = c; return *this; }
	AttrText& NormalInk(Color c)                    { normalink = c; return *this; }
	AttrText& Paper(Color c)                        { paper = c; return *this; }
	AttrText& SetFont(Font f)                       { font = f; return *this; }
	AttrText& Align(int a)                          { align = a; return *this; }
	AttrText& Left()                                { return Align(ALIGN_LEFT); }
	AttrText& Center()                              { return Align(ALIGN_CENTER); }
	AttrText& Right()                               { return Align(ALIGN_RIGHT); }
	AttrText& SetImage(const Image& m, int spc = 4) { img = m; imgspc = spc; return *this; }

	operator Value() const;
	AttrText(const Value& v);

	AttrText(const char *text);
	AttrText(const wchar *text);
	AttrText(const WString& text);
	AttrText(const String& text);
	AttrText()                                      { Init(); }

private:
	void Init();
};

const Display& StdDisplay();
const Display& StdCenterDisplay();
const Display& StdRightDisplay();

const Display& ColorDisplay();
const Display& SizeTextDisplay();
const Display& ImageDisplay();
const Display& FittedImageDisplay();
const Display& CenteredImageDisplay();
const Display& CenteredHighlightImageDisplay();
const Display& DrawingDisplay();

class ColorDisplayNull : public Display {
public:
#ifdef flagSO
	ColorDisplayNull(String nulltext = Null);
	virtual ~ColorDisplayNull();
#else
	ColorDisplayNull(String nulltext = Null) : nulltext(nulltext) {}
#endif
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
private:
	String nulltext;
};

class DisplayWithIcon : public Display {
	const Display *display;
	Image icon;
	int   lspc;
public:
	virtual void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                             Color ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	
	void SetIcon(const Image& img, int spc = 4)             { icon = img; lspc = spc; }
	void SetDisplay(const Display& d)                       { display = &d; }
	void Set(const Display& d, const Image& m, int spc = 4) { SetIcon(m, spc); SetDisplay(d); }
	
	DisplayWithIcon();
};

class PaintRect : Moveable<PaintRect> {
protected:
	Value          value;
	const Display *display;

public:
	void     Paint(Draw& w, const Rect& r,
	               Color ink = SColorText, Color paper = SColorPaper, dword style = 0) const;
	void     Paint(Draw& w, int x, int y, int cx, int cy,
		           Color ink = SColorText, Color paper = SColorPaper, dword style = 0) const;
	Size     GetStdSize() const;
	Size     RatioSize(int cx, int cy) const;
	Size     RatioSize(Size sz) const              { return RatioSize(sz.cx, sz.cy); }

	void     SetDisplay(const Display& d)          { display = &d; }
	void     SetValue(const Value& v)              { value = v; }
	void     Set(const Display& d, const Value& v) { display = &d; value = v; }
	void     Clear()                               { display = NULL; }

	const Value&   GetValue() const                { return value; }
	const Display& GetDisplay() const              { return *display; }

	operator bool() const                          { return display; }

	PaintRect();
	PaintRect(const Display& display);
	PaintRect(const Display& display, const Value& val);
};
