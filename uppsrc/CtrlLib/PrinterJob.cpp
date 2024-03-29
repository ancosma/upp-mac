#include "CtrlLib.h"

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#pragma  comment(lib, "comdlg32.lib")

#endif
#else

#include <PdfDraw/PdfDraw.h>

#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#ifndef PLATFORM_WINCE

#include <commdlg.h>
#pragma  comment(lib, "comdlg32.lib")

struct Win32PrintDlg_ : PRINTDLG {
	Win32PrintDlg_() {
		memset(this, 0, sizeof(PRINTDLG));
		lStructSize = sizeof(PRINTDLG);
	}
	~Win32PrintDlg_() {
		if(hDevMode)
			::GlobalFree(hDevMode);
		if(hDevNames)
			::GlobalFree(hDevNames);
	}
};

PrinterJob::PrinterJob(const char *_name)
{
	name = _name;
	landscape = false;
	from = to = 1;
	current = 1;
}

PrinterJob::~PrinterJob()
{
}

bool PrinterJob::Execute0(bool dodlg)
{
	pdlg = new Win32PrintDlg_;
	PRINTDLG& dlg = *pdlg;
	dlg.Flags = PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_HIDEPRINTTOFILE|PD_RETURNDEFAULT;
	dlg.nFromPage = current;
	dlg.nToPage = current;
	dlg.nMinPage = from;
	dlg.nMaxPage = to;
	if(from != to)
		dlg.Flags |= PD_ALLPAGES;
	dlg.hwndOwner = GetActiveWindow();
	dlg.Flags |= PD_RETURNDEFAULT;
	dlg.nCopies = 1;
	if(!PrintDlg(&dlg)) return false;
	if(dlg.hDevMode) {
		DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(dlg.hDevMode);
		pDevMode->dmOrientation = landscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
		::GlobalUnlock(dlg.hDevMode);
	}
	HDC hdc;
	if(dodlg) {
		dlg.Flags = PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_HIDEPRINTTOFILE|PD_RETURNDC;
		Vector< Ptr<Ctrl> > disabled = DisableCtrls(Ctrl::GetTopCtrls());
		bool b = PrintDlg(&dlg);
		EnableCtrls(disabled);
		if(!b) return false;
		hdc = dlg.hDC;
	}
	else {
		DEVNAMES *p = (DEVNAMES *)::GlobalLock(dlg.hDevNames);
		const char *driver = (const char *)p + p->wDriverOffset;
		const char *device = (const char *)p + p->wDeviceOffset;
		if(dlg.hDevMode) {
			DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(dlg.hDevMode);
			hdc = CreateDC(driver, device, NULL, pDevMode);
			::GlobalUnlock(dlg.hDevMode);
		}
		else
			hdc = CreateDC(driver, device, NULL, NULL);
	}
	if(dlg.hDevMode)
		::GlobalFree(dlg.hDevMode);
	if(dlg.hDevNames)
		::GlobalFree(dlg.hDevNames);
	if(hdc) {
		draw = new PrintDraw(hdc, Nvl(name, Ctrl::GetAppName()));
		page.Clear();
		if(!(dlg.Flags & PD_PAGENUMS)) {
			dlg.nFromPage = dlg.nMinPage;
			dlg.nToPage = dlg.nMaxPage;
		}
		for(int c = 0; c < ((dlg.Flags & PD_COLLATE) ? dlg.nCopies : 1); c++)
			for(int i = dlg.nFromPage - 1; i <= dlg.nToPage - 1; i++)
				for(int c = 0; c < ((dlg.Flags & PD_COLLATE) ? 1 : dlg.nCopies); c++)
					page.Add(i);
		return true;
	}
	return false;
}

bool PrinterJob::Execute()
{
	return Execute0(true);
}

Draw& PrinterJob::GetDraw()
{
	if(!draw) {
		Execute0(false);
		if(!draw)
			draw = new NilDraw;
	}
	return *draw;
}

PrinterJob& PrinterJob::MinMaxPage(int minpage, int maxpage)
{
	from = minpage + 1;
	to = maxpage + 1;
	return *this;
}

PrinterJob& PrinterJob::CurrentPage(int i)
{
	current = i + 1;
	return *this;
}

#endif

#endif

#ifdef PLATFORM_POSIX

String System(const char *cmd, const String& in)
{
	String ofn = GetTempFileName();
	String ifn = GetTempFileName();
	SaveFile(ifn, in);
	String c = cmd;
	c << " >" << ofn;
	if(in.GetCount())
		c << " <" << ifn;
	system(c);
	String q = LoadFile(ofn);
	FileDelete(ofn);
	FileDelete(ifn);
	return q;
}

String System(const char *cmd)
{
	return System(cmd, Null);
}

struct PrinterDlg : WithPrinterLayout<TopWindow> {
	void FillOpt(const String& s, const char *id, DropList& dl);
	void SyncPrinterOptions();

	typedef PrinterDlg CLASSNAME;

	PrinterDlg();
};

void PrinterDlg::FillOpt(const String& s, const char *id, DropList& dl)
{
	int a = s.Find('/');
	int b = s.Find(':');
	if(a > 0 && b > a) {
		String opt = ToLower(s.Mid(0, a));
		if(opt == id) {
			Vector<String> v = Split(~s + b + 1, ' ');
			dl.Enable();
			for(int i = 0; i < v.GetCount(); i++) {
				String o = v[i];
				if(o[0] == '*') {
					o = o.Mid(1);
					dl <<= o;
				}
				dl.Add(o);
			}
		}
	}
}

void PrinterDlg::SyncPrinterOptions()
{
	Vector<String> l = Split(System("lpoptions -l -d " + String(~printer)), '\n');
	paper.Disable();
	paper.Clear();
	slot.Disable();
	slot.Clear();
	for(int i = 0; i < l.GetCount(); i++) {
		FillOpt(l[i], "pagesize", paper);
		FillOpt(l[i], "inputslot", slot);
	}
}

PrinterDlg::PrinterDlg()
{
	CtrlLayoutOKCancel(*this, "Print");
	printer <<= THISBACK(SyncPrinterOptions);
	npage.Add(1);
	npage.Add(2);
	npage.Add(4);
	npage.Add(6);
	npage.Add(9);
	npage.Add(16);
	npage <<= 1;
	copies <<= 1;
	landscape <<= 0;
	range <<= 0;
	Vector<String> l = Split(System("lpstat -a"), '\n');
	for(int i = 0; i < l.GetCount(); i++) {
		Vector<String> w = Split(l[i], ' ');
		if(w.GetCount())
			printer.Add(w[0]);
	}
	String h = System("lpstat -d");
	int q = h.Find(':');
	if(q >= 0) {
		String p = h.Mid(q + 1);
		if(printer.HasKey(p)) {
			printer <<= p;
			SyncPrinterOptions();
			return;
		}
	}
	if(printer.GetCount()) {
		printer.SetIndex(0);
		SyncPrinterOptions();
	}
}

PrinterJob::PrinterJob(const char *_name)
{
	name = _name;
	from = to = 1;
	current = 1;
}

PrinterJob::~PrinterJob()
{
}

bool PrinterJob::Execute0(bool dodlg)
{
	struct {
		const char *name;
		int   cx, cy;
	}
	pg[] = {
		{ "A0", 841, 1189 },
		{ "A1", 594, 841 },
		{ "A2", 420, 594 },
		{ "A3", 297, 420 },
		{ "A4", 210, 297 },
		{ "A5", 148, 210 },
		{ "A6", 105, 148 },
		{ "A7", 74, 105 },
		{ "A8", 52, 74 },
		{ "A9", 37, 52 },
		{ "B0", 1030, 1456 },
		{ "B1", 728, 1030 },
		{ "B10", 32, 45 },
		{ "B2", 515, 728 },
		{ "B3", 364, 515 },
		{ "B4", 257, 364 },
		{ "B5", 182, 257 },
		{ "B6", 128, 182 },
		{ "B7", 91, 128 },
		{ "B8", 64, 91 },
		{ "B9", 45, 64 },
		{ "C5E", 163, 229 },
		{ "Comm10E", 105, 241 },
		{ "DLE", 110, 220 },
		{ "Executive", 191, 254 },
		{ "Folio", 210, 330 },
		{ "Ledger", 432, 279 },
		{ "Legal", 216, 356 },
		{ "Letter", 216, 279 },
		{ "Tabloid", 279, 432 }
	};

	PrinterDlg dlg;
	dlg.from <<= from + 1;
	dlg.to <<= to + 1;
	dlg.from.Min(from + 1).Max(to + 1);
	dlg.to.Min(from + 1).Max(to + 1);
	if(dodlg)
		if(dlg.Run() != IDOK)
			return false;
	options.Clear();
	options << " -d " << ~dlg.printer;
	options << " -o media=" << ~dlg.paper << "," << ~dlg.slot;
	if(dlg.landscape)
		options << " -o landscape";
	options << " -o number-up=" << ~dlg.npage;
	options << " -n " << ~dlg.copies;
	if(dlg.collate)
		options << " -o Collate=True";
	page.Clear();
	switch(dlg.range) {
	case 0:
		for(int i = from; i <= to; i++)
			page.Add(i);
		break;
	case 1:
		page.Add(current);
		break;
	case 2:
		for(int i = (int)~dlg.from - 1; i <= (int)~dlg.to - 1; i++)
			page.Add(i);
		break;
	}
	pgsz = Size(5100, 6600);
	for(int i = 0; i < __countof(pg); i++)
		if((String)~dlg.paper == pg[i].name) {
			pgsz.cx = 6000 * pg[i].cx / 254;
			pgsz.cy = 6000 * pg[i].cy / 254;
		}
	return true;
}

bool PrinterJob::Execute()
{
	return Execute0(true);
}

struct PrinterDraw : PdfDraw {
	String options;

	PrinterDraw(Size sz) : PdfDraw(sz) {}
	~PrinterDraw() {
		System("lp " + options + " -", Finish());
	}
};

Draw& PrinterJob::GetDraw()
{
	PrinterDraw *pd = new PrinterDraw(pgsz);
	pd->options = options;
	draw = pd;
	return *pd;
}

PrinterJob& PrinterJob::MinMaxPage(int minpage, int maxpage)
{
	from = minpage;
	to = maxpage;
	return *this;
}

PrinterJob& PrinterJob::CurrentPage(int i)
{
	current = i;
	return *this;
}

#endif

END_UPP_NAMESPACE
