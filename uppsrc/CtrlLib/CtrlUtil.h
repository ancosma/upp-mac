void Animate(Ctrl& c, const Rect& target, int type = -1);
void Animate(Ctrl& c, int x, int y, int cx, int cy, int type = -1);

bool CtrlLibDisplayError(const Value& ev);

bool EditText(String& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditText(String& s, const char *title, const char *label, int maxlen = 0);

bool EditText(WString& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditText(WString& s, const char *title, const char *label, int maxlen = 0);

void Show2(Ctrl& ctrl1, Ctrl& ctrl, bool show = true);
void Hide2(Ctrl& ctrl1, Ctrl& ctrl);

#ifndef PLATFORM_WINCE //TODO?
void UpdateFile(const char *filename);
void SelfUpdate();
bool SelfUpdateSelf();
#endif

void WindowsList();
void WindowsMenu(Bar& bar);

class DelayCallback : public Pte<DelayCallback> {
	Callback target;
	int      delay;

public:
	void     Invoke();
	void     operator<<=(Callback x)  { target = x; }
	void     SetDelay(int ms)         { delay = ms; }
	Callback Get()                    { return callback(this, &DelayCallback::Invoke); }
	Callback operator~()              { return Get(); }
	operator Callback()               { return Get(); }

	DelayCallback()                   { delay = 2000; }
	~DelayCallback()                  { KillTimeCallback(this); }
};

#ifdef PLATFORM_WIN32
struct Win32PrintDlg_;
#endif

#ifndef PLATFORM_WINCE

class PrinterJob {
#ifdef PLATFORM_WIN32
	One<Win32PrintDlg_> pdlg;
#endif
#ifdef PLATFORM_X11
	Size                pgsz;
#endif
	One<Draw>           draw;
	Vector<int>         page;
	int                 from, to, current;
	bool                landscape;
	String              name;
	String              options;
	bool                Execute0(bool dodlg);

public:
	Draw&               GetDraw();
	operator            Draw&()                         { return GetDraw(); }
	const Vector<int>&  GetPages() const                { return page; }
	int                 operator[](int i) const         { return page[i]; }
	int                 GetPageCount() const            { return page.GetCount(); }

	bool                Execute();

	PrinterJob& Landscape(bool b = true)                { landscape = b; return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage);
	PrinterJob& PageCount(int n)                        { return MinMaxPage(0, n - 1); }
	PrinterJob& CurrentPage(int currentpage);
	PrinterJob& Name(const char *_name)                 { name = _name; return *this; }

	PrinterJob(const char *name = NULL);
	~PrinterJob();
};

#endif

#ifdef PLATFORM_X11

class TrayIcon : Ctrl {
	virtual bool HookProc(XEvent *event);
	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void LeftDouble(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

private:
	void AddToTray();

	Window traywin;
	Image  icon;
	void DoMenu(Bar& bar);
	void Call(int code, unsigned long d1, unsigned long d2, unsigned long d3);
	void Message(int type, const char *title, const char *text, int timeout);

public:
	virtual void    LeftDown();
	virtual void    LeftUp();
	virtual void    LeftDouble();
	virtual void    Menu(Bar& bar);
	virtual void    BalloonLeftDown();
	virtual void    BalloonShow();
	virtual void    BalloonHide();
	virtual void    BalloonTimeout();

	Callback        WhenLeftDown;
	Callback        WhenLeftUp;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;
	Callback        WhenBalloonLeftDown;
	Callback        WhenBalloonShow;
	Callback        WhenBalloonHide;
	Callback        WhenBalloonTimeout;

	void            Break()                                { EndLoop(0); }
	void            Run()                                  { EventLoop(this); }

	void            Show(bool b = true);
	void            Hide()                                 { Show(false); }
	bool            IsVisible() const                      { return true; }

	void            Info(const char *title, const char *text, int timeout = 10)    { Message(1, title, text, timeout); }
	void            Warning(const char *title, const char *text, int timeout = 10) { Message(2, title, text, timeout); }
	void            Error(const char *title, const char *text, int timeout = 10)   { Message(3, title, text, timeout); }

	TrayIcon&  Icon(const Image &img)                      { icon = img; Refresh(); return *this; }
	TrayIcon&  Tip(const char *text)                       { Ctrl::Tip(text); return *this; }

	typedef TrayIcon CLASSNAME;

	TrayIcon();
};

#endif

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

class TrayIcon : private Ctrl {
	struct NotifyIconOld {
		dword sz;
		HWND  hwnd;
		dword id;
		dword flags;
		dword message;
		HICON icon;
		char  tip[64];
	};
	struct NotifyIconNew {
		dword sz;
		HWND  hwnd;
		dword id;
		dword flags;
		dword message;
		HICON icon;
		char  tip[128];

		dword state;
		dword statemask;
		char  info[256];
		dword timeout;
		char  title[64];
		dword infoflags;
	};

	Image          icon;
	bool           visible;
	String         tip;
	NotifyIconNew  nid;
	HWND           hwnd;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Notify(dword msg);
	void DoMenu(Bar& bar);
	void Message(int type, const char *title, const char *text, int timeout = 10);

public:
	virtual void    Menu(Bar& bar);
	virtual void    LeftDown();
	virtual void    LeftUp();
	virtual void    LeftDouble();
	virtual void    BalloonLeftDown();
	virtual void    BalloonShow();
	virtual void    BalloonHide();
	virtual void    BalloonTimeout();

	Callback        WhenLeftDown;
	Callback        WhenLeftUp;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;
	Callback        WhenBalloonLeftDown;
	Callback        WhenBalloonShow;
	Callback        WhenBalloonHide;
	Callback        WhenBalloonTimeout;

	void            Show(bool b = true);
	void            Hide()                                 { Show(false); }
	bool            IsVisible() const                      { return visible; }
	void            Break()                                { EndLoop(0); }
	void            Run()                                  { EventLoop(this); }

	void            Info(const char *title, const char *text, int timeout = 10)    { Message(1, title, text, timeout); }
	void            Warning(const char *title, const char *text, int timeout = 10) { Message(2, title, text, timeout); }
	void            Error(const char *title, const char *text, int timeout = 10)   { Message(3, title, text, timeout); }

	TrayIcon&  Icon(const Image &img);
	TrayIcon&  Tip(const char *text);

	typedef TrayIcon CLASSNAME;

	TrayIcon();
	~TrayIcon();
};

class FileSelector {
protected:
	struct FileType : Moveable<FileType> {
		String name;
		String ext;
	};

	Vector<FileType> type;

	int    activetype;
	String activedir;
	String defext;
	bool   rdonly;
	bool   multi;
	bool   readonly;
	bool   asking;

	Vector<String> filename;

public:
	void Serialize(Stream& s);

	void New()                                   { filename.Clear(); }
	bool IsNew() const                           { return filename.IsEmpty(); }
	bool Execute(bool open, const char *title = NULL);
	bool ExecuteOpen(const char *title = NULL)   { return Execute(true, title); }
	bool ExecuteSaveAs(const char *title = NULL) { return Execute(false, title); }

	String Get() const;
	void  Set(const String& s)                   { filename.At(0) = s; }

	operator String() const                      { return Get(); }
	void operator=(const String& s)              { Set(s); }

	String operator~() const                     { return Get(); }
	void operator<<=(const String& s)            { Set(s); }

	int   GetCount() const                       { return filename.GetCount(); }
	const String& operator[](int i) const        { return filename[i]; }

	bool   GetReadOnly() const                   { return readonly; }
	String GetActiveDir() const                  { return activedir; }

	FileSelector& Type(const char *name, const char *ext);
	FileSelector& AllFilesType();
	FileSelector& ActiveDir(const String& dir)   { activedir = dir; return *this; }
	FileSelector& ActiveType(int i)              { activetype = i; return *this;  }
	FileSelector& DefaultExt(const char *ext)    { defext = ext; return *this; }
	FileSelector& Multi(bool b = true)           { multi = b; return *this; }
	FileSelector& ReadOnlyOption(bool b = true)  { rdonly = b; return *this; }
	FileSelector& Asking(bool b = true)          { asking = b; return *this; }
	FileSelector& NoAsking()                     { return Asking(false); }

	FileSelector();
};

#endif
#endif

#ifdef PLATFORM_X11
typedef FileSel FileSelector;
#endif

class CtrlRetriever {
public:
	struct Item {
		virtual void Retrieve() = 0;
		virtual ~Item() {}
	};

private:
	struct CtrlItem0 : Item {
		Ctrl  *ctrl;
	};

	template <class T>
	struct CtrlItem : CtrlItem0 {
		T     *value;

		virtual void Retrieve()  { *value = ~*ctrl; }
		virtual ~CtrlItem() {}
	};

	Array<Item> item;

public:
	void Put(Item *newitem)                       { item.Add(newitem); }

	template <class T>
	void Put(Ctrl& ctrl, T& val);

	template <class T>
	CtrlRetriever& operator()(Ctrl& ctrl, T& val) { Put(ctrl, val); return *this; }

	void Retrieve();

	Callback operator<<=(Callback cb);
};

template <class T>
void CtrlRetriever::Put(Ctrl& ctrl, T& val)
{
	CtrlItem<T> *m = new CtrlItem<T>();
	m->ctrl = &ctrl;
	m->value = &val;
	ctrl <<= val;
	Put(m);
}

class IdCtrls {
	struct Item {
		Id    id;
		Ctrl *ctrl;
	};
	Array<Item> item;

public:
	void        Reset()                              { item.Clear(); }

	void        Add(Id id, Ctrl& ctrl);
	IdCtrls&    operator()(Id id, Ctrl& ctrl)        { Add(id, ctrl); return *this; }
	int         GetCount() const                     { return item.GetCount(); }
	Ctrl&       operator[](int i)                    { return *item[i].ctrl; }
	const Ctrl& operator[](int i) const              { return *item[i].ctrl; }
	Id          operator()(int i) const              { return item[i].id; }
};

void Set(ArrayCtrl& array, int ii, IdCtrls& m);
void Get(ArrayCtrl& array, int ii, IdCtrls& m);

void   UpdateSetDir(const char *path);
void   UpdateSetUpdater(const char *exename);
String UpdateGetDir();

void UpdateFile(String dst, String src);

void MemoryProfileInfo();

struct sPaintRedirectCtrl : Ctrl {
	Ctrl *ctrl;
	
	virtual void Paint(Draw& w) {
		ctrl->Paint(w);
	}
};
