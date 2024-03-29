#include "CtrlLib.h"

NAMESPACE_UPP

CH_VALUE(ViewEdge, CtrlsImg::VE());

Value EditFieldEdge()
{
	return EditField::StyleDefault().edge[0];
}

CtrlFrame& EditFieldFrame()
{
	static LookFrame m(EditFieldEdge);
	return m;
}

CtrlFrame& ViewFrame()
{
	static LookFrame m(ViewEdge);
	return m;
}

bool IsWCh(int c)
{
	return IsLeNum(c) || c == '_';
}

bool TextArrayOps::GetWordSelection(int c, int& l, int& h)
{
	if(IsWCh(GetChar(c))) {
		l = h = c;
		while(l > 0 && IsWCh(GetChar(l - 1)))
			l--;
		while(h < GetLength() && IsWCh(GetChar(h)))
			h++;
		if(h != c)
			while(h < GetLength() && GetChar(h) == ' ')
				h++;
		return true;
	}
	return false;
}

int TextArrayOps::GetNextWord(int cursor)
{
	bool a = IsWCh(GetChar(cursor));
	int n = 0;
	int c = cursor;
	while(c <= GetLength() && IsWCh(GetChar(c)) == a) {
		if(++n > 10000) return cursor;
		c++;
	}
	return c;
}

int TextArrayOps::GetPrevWord(int cursor)
{
	int n = 0;
	int c = cursor;
	if(c == 0) return 0;
	bool a = IsWCh(GetChar(c - 1));
	while(c > 0 && IsWCh(GetChar(c - 1)) == a) {
		if(++n > 10000) return cursor;
		c--;
	}
	return c;
}

Rect LookMargins(const Rect& r, const Value& ch)
{
	Rect m = ChMargins(ch);
	int fcy = GetStdFontCy();
	if(m.top + m.bottom + fcy > r.GetHeight())
		m.top = m.bottom = max((r.GetHeight() - fcy) / 2, 0);
	return m;
}

void LookFrame::FrameLayout(Rect& r)
{
	Rect m = LookMargins(r, Get());
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void LookFrame::FramePaint(Draw& w, const Rect& r)
{
	ChPaintEdge(w, r, Get());
}

void LookFrame::FrameAddSize(Size& sz)
{
	Rect m = ChMargins(Get());
	sz.cx += m.left + m.right;
	sz.cy += m.top + m.bottom;
}

void ActiveEdgeFrame::FrameLayout(Rect& r)
{
	Rect m = LookMargins(r, edge[0]);
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void ActiveEdgeFrame::FramePaint(Draw& w, const Rect& r)
{
	int i = 0;
	if(ctrl) {
		i = !ctrl->IsEnabled() ? CTRL_DISABLED
		    : ctrl->HasFocus() ? CTRL_PRESSED
		    : mousein ? CTRL_HOT
		    : CTRL_NORMAL;
	}
	ChPaintEdge(w, r, edge[i]);
}

void ActiveEdgeFrame::FrameAddSize(Size& sz)
{
	Rect m = ChMargins(edge[0]);
	sz.cx += m.left + m.right;
	sz.cy += m.top + m.bottom;
}

void ActiveEdgeFrame::Set(const Ctrl *ctrl_, const Value *edge_, bool active)
{
	ctrl = active ? ctrl_ : NULL;
	edge = edge_;
}

CH_STYLE(EditField, Style, StyleDefault)
{
	paper = SColorPaper();
	disabled = SColorFace();
	focus = paper;
	invalid = Blend(paper, Color(255, 0, 0), 32);
	text = SColorText();
	textdisabled = SColorDisabled();
	selected = SColorHighlight();
	selectedtext = SColorHighlightText();
	for(int i = 0; i < 4; i++)
		edge[i] = CtrlsImg::EFE();
	activeedge = false;
	vfm = 2;
}

bool EditField::FrameIsEdge()
{
	return &GetFrame() == &edge;
}

void EditField::SyncEdge()
{
	if(FrameIsEdge() && style->activeedge)
		RefreshFrame();
}

void EditField::MouseEnter(Point p, dword keyflags)
{
	edge.Mouse(true);
	SyncEdge();
}

void EditField::MouseLeave()
{
	edge.Mouse(false);
	SyncEdge();
}

EditField& EditField::SetStyle(const Style& s)
{
	style = &s;
	edge.Set(this, style->edge, style->activeedge);
	RefreshLayout();
	RefreshFrame();
	return *this;
}

void EditField::CancelMode()
{
	keep_selection = false;
	selclick = false;
	dropcaret.Clear();
}

int EditField::GetTextCx(const wchar *txt, int n, bool password, Font fnt)
{
	FontInfo fi = fnt.Info();
	if(password)
		return n * fi['*'];
	const wchar *s = txt;
	int x = 0;
	while(n--)
		x += fi[*s++];
	return x;
}

int  EditField::GetCaret(int cursor)
{
	return GetTextCx(text, cursor, password, font);
}

int  EditField::GetViewHeight(Font font)
{
	Size sz(0, 0);
	EditFieldFrame().FrameAddSize(sz);
	return font.Info().GetHeight() + (sz.cy <= 2 ? 4 : sz.cy <= 4 ? 2 : 0);
}

int  EditField::GetStdHeight(Font font)
{
	Size sz = Size(10, GetViewHeight());
	EditFieldFrame().FrameAddSize(sz);
	return sz.cy;
}

Size EditField::GetMinSize() const
{
	return AddFrameSize(10, font.Info().GetHeight() + 4);
}

int  EditField::GetCursor(int posx)
{
	posx -= 2;
	if(posx <= 0) return 0;

	FontInfo fi = font.Info();
	int count = text.GetLength();
	if(password)
		return min((posx + fi['*'] / 2) / fi['*'], count);

	int x = 0;
	const wchar *s = text;
	int i = 0;
	while(i < count) {
		int witdh = fi[*s];
		if(posx < x + witdh / 2)
			break;
		x += witdh;
		s++; i++;
	}
	return i;
}

Image EditField::CursorImage(Point, dword)
{
	return Image::IBeam();
}

int  EditField::GetTy()
{
	return (GetSize().cy + 1 - font.Info().GetHeight()) / 2;
}

void EditField::Paints(Draw& w, int& x, int fcy, const wchar *&txt,
					   Color ink, Color paper, int n, bool password, Font fnt)
{
	if(n < 0) return;
	int cx = GetTextCx(txt, n, password, font);
	w.DrawRect(x, 0, cx, fcy, paper);
	if(password) {
		String h;
		h.Cat('*', n);
		w.DrawText(x, 0, ~h, fnt, ink, n);
	}
	else
		w.DrawText(x, 0, txt, fnt, ink, n);
	txt += n;
	x += cx;
}

void EditField::Paint(Draw& w)
{
	Size sz = GetSize();
	bool enabled = IsShowEnabled();
	Color paper = enabled && !IsReadOnly() ? (HasFocus() ? style->focus : style->paper) : style->disabled;
	if(nobg)
		paper = Null;
	Color ink = enabled ? style->text : style->textdisabled;
	if(enabled && (convert && convert->Scan(text).IsError() || errorbg))
		paper = style->invalid;
	int fcy = font.Info().GetHeight();
	int yy = GetTy();
	w.DrawRect(0, 0, 2, sz.cy, paper);
	w.DrawRect(0, 0, sz.cx, yy, paper);
	w.DrawRect(0, yy + fcy, sz.cx, sz.cy - yy - fcy, paper);
	w.DrawRect(sz.cx - 2, 0, 2, sz.cy, paper);
	w.Clipoff(2, yy, sz.cx - 4, fcy);
	int x = -sc;
	bool ar = alignright && !HasFocus();
	if(IsNull(text) && (!IsNull(nulltext) || !IsNull(nullicon))) {
		const wchar *txt = nulltext;
		if(!IsNull(nullicon)) {
			int icx = nullicon.GetWidth();
			w.DrawRect(x, 0, icx + 4, fcy, paper);
			w.DrawImage(x, (fcy - nullicon.GetHeight()) / 2, nullicon);
			x += icx + 4;
		}
		Paints(w, x, fcy, txt, nullink, paper, nulltext.GetLength(), false, nullfont);
	}
	else {
		const wchar *txt = text;
		if(ar) {
			x = sz.cx - 4 - GetTextCx(text, text.GetLength(), password, font);
			w.DrawRect(0, 0, x, fcy, paper);
		}
		int l, h;
		if(GetSelection(l, h)) {
			Paints(w, x, fcy, txt, ink, paper, l, password, font);
			Paints(w, x, fcy, txt, enabled ? style->selectedtext : paper,
			                       enabled ? style->selected : ink, h - l, password, font);
			Paints(w, x, fcy, txt, ink, paper, text.GetLength() - h, password, font);
		}
		else
			Paints(w, x, fcy, txt, ink, paper, text.GetLength(), password, font);
	}
	if(!ar)
		w.DrawRect(x, 0, 9999, fcy, paper);
	w.End();
	DrawTiles(w, dropcaret, CtrlImg::checkers());
}

bool EditField::GetSelection(int& l, int& h) const
{
	if(anchor < 0 || anchor == cursor) {
		l = h = cursor;
		return false;
	}
	if(anchor < cursor) {
		l = anchor;
		h = cursor;
	}
	else {
		l = cursor;
		h = anchor;
	}
	return true;
}

bool EditField::IsSelection() const
{
	return anchor >= 0 && anchor != cursor;
}

void EditField::SyncCaret()
{
	FontInfo fi = font.Info();
	SetCaret(GetCaret(cursor) - sc + 2 - fi.GetRightSpace('o') + fi.GetLeftSpace('o'), GetTy(),
	         1, min(GetSize().cy - 2 * GetTy(), fi.GetHeight()));
}

void EditField::Finish(bool refresh)
{
	if(anchor > text.GetLength()) anchor = text.GetLength();
	if(cursor > text.GetLength()) cursor = text.GetLength();
	if(cursor < 0) cursor = 0;
	Size sz = GetSize();
	if(autosize) {
		Rect r = GetRect();
		int mw = min(r.Width(), Draw::GetStdFontSize().cx);
		sz.cx = GetCaret(text.GetLength()) + 4;
		sz = AddFrameSize(sz);
		if(GetParent())
			sz.cx = min(sz.cx, GetParent()->GetSize().cx - r.left);
		sz.cx = minmax(sz.cx, mw, autosize);
		if(sz.cx != r.Width())
			LeftPos(r.left, sz.cx);
		sz = GetSize();
	}
	sz.cx -= 2;
	if(sz.cx <= 0) return;
	int x = GetCaret(cursor);
	int wx = x + font.Info().GetRightSpace('o');
	if(wx > sz.cx + sc - 1) {
		sc = wx - sz.cx + 1;
		Refresh();
	}
	if(x < sc) {
		sc = x;
		Refresh();
	}
	if(refresh)
		Refresh();
	SyncCaret();
}

void EditField::Layout()
{
	Ctrl::Layout();
	sc = 0;
	Finish();
}

void EditField::GotFocus()
{
	if(autoformat && IsEditable() && !IsNull(text) && inactive_convert) {
		Value v = convert->Scan(text);
		if(!v.IsError()) {
			WString s = convert->Format(v);
			if(s != text) text = s;
		}
	}
	if(!keep_selection) {
		anchor = 0;
		cursor = text.GetLength();
	}
	Finish();
	SyncEdge();
}

void EditField::LostFocus()
{
	if(autoformat && IsEditable() && !IsNull(text)) {
		Value v = convert->Scan(text);
		if(!v.IsError()) {
			const Convert * cv = inactive_convert ? inactive_convert : convert;
			WString s = cv->Format(v);
			if(s != text) text = s;
		}
	}
	if(!keep_selection) {
		anchor = -1;
		cursor = sc = 0;
	}
	Refresh();
	SyncEdge();
}

void EditField::LeftDown(Point p, dword flags)
{
	int c = GetCursor(p.x + sc);
	if(!HasFocus()) {
		SetFocus();
		if(clickselect) {
			SetSelection();
			Finish();
			return;
		}
		sc = 0;
		Move(c);
	}
	int l, h;
	selclick = false;
	if(GetSelection(l, h) && c >= l && c < h) {
		selclick = true;
		return;
	}
	SetCapture();
	Move(c, flags & K_SHIFT);
	Finish();
}

void EditField::MiddleDown(Point p, dword flags)
{
	if(IsReadOnly())
		return;
	if(AcceptText(Selection())) {
		WString w = GetWString(Selection());
		selclick = false;
		LeftDown(p, flags);
		Insert(w);
		Action();
		Finish();
	}
}

void EditField::LeftUp(Point p, dword flags)
{
	int c = GetCursor(p.x + sc);
	int l, h;
	if(GetSelection(l, h) && c >= l && c < h && !HasCapture() && selclick)
		Move(c, false);
	Finish();
	selclick = false;
}

void EditField::LeftDouble(Point p, dword flags)
{
	int l, h;
	if(GetWordSelection(cursor, l, h))
		SetSelection(l, h);
}

void EditField::LeftTriple(Point p, dword keyflags)
{
	anchor = 0;
	cursor = text.GetLength();
	Finish();
}

void EditField::MouseMove(Point p, dword flags)
{
	if(!HasCapture()) return;
	Move(GetCursor(p.x + sc), true);
	Finish();
}

void EditField::SaveUndo()
{
	undotext = text;
	undoanchor = anchor;
	undocursor = cursor;
}

void EditField::Move(int newpos, bool select)
{
	bool refresh = anchor >= 0;
	if(select) {
		if(anchor < 0) anchor = cursor;
		refresh = true;
	}
	else
		anchor = -1;
	cursor = newpos;
	Finish(refresh);
	if(select)
		SetSelectionSource(ClipFmtsText());
}

void EditField::SetSelection(int l, int h)
{
	if(l < h) {
		anchor = max(l, 0);
		cursor = min(h, text.GetLength());
	}
	else {
		cursor = l;
		anchor = -1;
	}
	Finish();
}

void EditField::CancelSelection()
{
	int l, h;
	if(GetSelection(l, h)) {
		cursor = l;
		anchor = -1;
		sc = 0;
		Finish();
	}
}

bool EditField::RemoveSelection()
{
	int l, h;
	if(!GetSelection(l, h)) {
		anchor = -1;
		return false;
	}
	SaveUndo();
	Remove(l, h - l);
	cursor = l;
	anchor = -1;
	sc = 0;
	return true;
}

void EditField::Copy()
{
	int l, h;
	if(password) return;
	if(!GetSelection(l, h)) {
		l = 0;
		h = text.GetLength();
	}
	WriteClipboardUnicodeText(text.Mid(l, h - l));
}

int EditField::Insert(int pos, const WString& itext)
{
	if(IsReadOnly()) return 0;
	WString ins;
	const wchar *s = itext;
	for(;;) {
		wchar chr = *s++;
		if(chr == '\t')
			ins.Cat(WString(' ', 4));
		else
		if(chr >= ' ') {
			chr = (*filter)(chr);
			if(chr) {
				chr = convert->Filter(chr);
				if(chr && (charset == CHARSET_UNICODE || FromUnicode(chr, charset, 0)))
					ins.Cat(chr);
			}
		}
		else
			break;
	}
	if(ins.GetCount() + text.GetCount() > maxlen) {
		BeepExclamation();
		return 0;
	}
	text.Insert(pos, ins);
	Update();
	return ins.GetLength();
}

void EditField::Remove(int pos, int n)
{
	if(IsReadOnly()) return;
	text.Remove(pos, n);
	Update();
}

void EditField::Insert(int chr)
{
	if(IsReadOnly()) return;
	if(initcaps && cursor == 0 && text.GetCount() == 0)
		chr = ToUpper(chr);
	cursor += Insert(cursor, WString(chr, 1));
	Finish();
}

void EditField::Insert(const WString& s)
{
	if(!RemoveSelection()) SaveUndo();
	cursor += Insert(cursor, s);
	Finish();
}

void EditField::DragAndDrop(Point p, PasteClip& d)
{
	if(IsReadOnly()) return;
	int c = GetCursor(p.x + sc);
	if(AcceptText(d)) {
		SaveUndo();
		int sell, selh;
		if(GetSelection(sell, selh)) {
			if(c >= sell && c < selh) {
				RemoveSelection();
				if(IsDragAndDropSource())
					d.SetAction(DND_COPY);
				c = sell;
			}
			else
			if(d.GetAction() == DND_MOVE && IsDragAndDropSource()) {
				if(c > sell)
					c -= selh - sell;
				RemoveSelection();
				d.SetAction(DND_COPY);
			}
		}
		int count = Insert(c, GetWString(d));
		SetFocus();
		SetSelection(c, c + count);
		Action();
		return;
	}
	if(!d.IsAccepted()) return;
	Rect dc(0, 0, 0, 0);
	if(c >= 0) {
		FontInfo fi = font.Info();
		int x = GetCaret(c);
		dc = RectC(x - sc + 2 - fi.GetRightSpace('o'), GetTy(),
		           1, min(GetSize().cy - 2 * GetTy(), fi.GetHeight()));
	}
	if(dc != dropcaret) {
		Refresh(dropcaret);
		dropcaret = dc;
		Refresh(dropcaret);
	}
}

void EditField::DragRepeat(Point p)
{
	if(IsReadOnly())
		return;
	Size sz = GetSize();
	int sd = min(sz.cx / 6, 16);
	int d = 0;
	if(p.x < sd)
		d = -3;
	if(p.x > sz.cx - sd)
		d = 3;
	int a = minmax((int)sc + minmax(d, -16, 16), 0, max(0, GetCaret(GetLength()) - sz.cx + 2));
	if(a != sc) {
		sc = a;
		Refresh();
		SyncCaret();
	}
}

void EditField::DragLeave()
{
	Refresh(dropcaret);
	dropcaret.Clear();
}

void EditField::LeftDrag(Point p, dword flags)
{
	if(password)
		return;
	int c = GetCursor(p.x + sc);
	Size ssz = StdSampleSize();
	int sell, selh;
	if(!HasCapture() && GetSelection(sell, selh) && c >= sell && c <= selh) {
		WString sel = text.Mid(sell, selh - sell);
		ImageDraw iw(ssz);
		iw.DrawText(0, 0, sel);
		iw.Alpha().DrawRect(ssz, Black);
		iw.Alpha().DrawText(0, 0, sel, StdFont(), White);
		VectorMap<String, ClipData> data;
		Append(data, sel);
		if(DoDragAndDrop(data, iw) == DND_MOVE) {
			CancelSelection();
			SaveUndo();
			Remove(sell, selh - sell);
			sc = 0;
			Finish();
			Action();
		}
	}
}

String EditField::GetSelectionData(const String& fmt) const
{
	int sell, selh;
	if(GetSelection(sell, selh))
		return GetTextClip(text.Mid(sell, selh - sell), fmt);
	return String();
}

void EditField::Undo()
{
	if(!IsEditable())
		return;
	Swap(undotext, text);
	Swap(undoanchor, anchor);
	Swap(undocursor, cursor);
	anchor = -1;
	UpdateAction();
	Finish();
}

void EditField::Cut()
{
	if(!IsEditable())
		return;
	Copy();
	RemoveSelection();
	Action();
	Finish();
}

void EditField::Paste()
{
	if(!IsEditable())
		return;
	Insert(ReadClipboardUnicodeText());
	Action();
	Finish();
}

void EditField::Erase()
{
	if(!IsEditable())
		return;
	if(!IsSelection())
		SelectAll();
	RemoveSelection();
	Finish();
}

void EditField::SelectAll()
{
	SetSelection();
	Finish();
}

void EditField::StdBar(Bar& menu) {
	menu.Add(t_("Undo"), THISBACK(Undo))
		.Key(K_ALT_BACKSPACE)
		.Key(K_CTRL_Z);
	menu.Separator();
	menu.Add(IsEditable() && IsSelection(), t_("Cut"), CtrlImg::cut(), THISBACK(Cut))
		.Key(K_SHIFT_DELETE)
		.Key(K_CTRL_X);
	menu.Add(IsSelection(), t_("Copy"), CtrlImg::copy(), THISBACK(Copy))
		.Key(K_CTRL_INSERT)
		.Key(K_CTRL_C);
	menu.Add(IsEditable()
		#ifdef PLATFORM_WIN32
			&& ::IsClipboardFormatAvailable(CF_TEXT)
		#endif
			,
			t_("Paste"), CtrlImg::paste(), THISBACK(Paste))
		.Key(K_SHIFT_INSERT)
		.Key(K_CTRL_V);
	menu.Add(IsEditable(), t_("Erase"), CtrlImg::remove(), THISBACK(Erase))
		.Key(K_DELETE);
	menu.Separator();
	menu.Add(GetLength(), t_("Select all"), THISBACK(SelectAll))
		.Key(K_CTRL_A);
}


void EditField::RightDown(Point p, dword keyflags)
{
	keep_selection = true;
	MenuBar::Execute(WhenBar);
	SetFocus();
	keep_selection = false;
}

bool EditField::Key(dword key, int rep)
{
	int q;
	bool h;
	String s;
	bool select = key & K_SHIFT;
	switch(key & ~K_SHIFT) {
	case K_LEFT:
		Move(cursor - 1, select);
		return true;
	case K_CTRL_LEFT:
		Move(GetPrevWord(cursor), select);
		return true;
	case K_CTRL_RIGHT:
		Move(GetNextWord(cursor), select);
		return true;
	case K_RIGHT:
		Move(cursor + 1, select);
		return true;
	case K_HOME:
		Move(0, select);
		return true;
	case K_END:
		Move(text.GetLength(), select);
		return true;
	}
	if(!IsEditable())
		return MenuBar::Scan(WhenBar, key);
	switch(key) {
	case K_BACKSPACE:
	case K_SHIFT|K_BACKSPACE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor == 0 || IsReadOnly()) return true;
		SaveUndo();
		cursor--;
		Remove(cursor, 1);
		Action();
		break;
	case K_CTRL_BACKSPACE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor == 0 || IsReadOnly()) return true;
		SaveUndo();
		q = cursor;
		h = IsWCh(text[--cursor]);
		while(cursor > 0 && IsWCh(text[cursor - 1]) == h)
			cursor--;
		Remove(cursor, q - cursor);
		Action();
		break;
	case K_DELETE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor >= text.GetLength()) return true;
		SaveUndo();
		Remove(cursor, 1);
		Action();
		break;
	case K_CTRL_DELETE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor >= text.GetLength()) return true;
		q = cursor;
		h = IsWCh(text[q]);
		while(IsWCh(text[q]) == h && q < text.GetLength()) q++;
		SaveUndo();
		Remove(cursor, q - cursor);
		Action();
		break;
	case K_ENTER:
		if(WhenEnter) {
			WhenEnter();
			return true;
		}
		return false;
	default:
		if(key >= ' ' && key < 65536 || key == K_SHIFT_SPACE) {
			if(!RemoveSelection()) SaveUndo();
			while(rep--)
				Insert(key == K_SHIFT_SPACE ? ' ' : key);
			Action();
			return true;
		}
		else
			return MenuBar::Scan(WhenBar, key);
	}
	Finish();
	return true;
}

void EditField::SetText(const WString& txt)
{
	if(text == txt) {
		Update();
		return;
	}
	text = txt;
	sc = 0;
	if(HasFocus()) {
		cursor = txt.GetLength();
		anchor = 0;
	}
	else {
		cursor = 0;
		anchor = -1;
	}
	Update();
	Finish();
}

void EditField::SetData(const Value& data)
{
	const Convert * cv = convert;
	if(!HasFocus() && inactive_convert)
		cv = inactive_convert;
	SetText((WString) cv->Format(data));
}

Value EditField::GetData() const
{
	return convert->Scan(text);
}

void EditField::Clear()
{
	SetText(WString());
	sc = cursor = 0;
}

void EditField::Reset()
{
	Clear();
	ClearModify();
	sc = 0;
	cursor = 0;
	anchor = -1;
	password = false;
	autoformat = true;
	clickselect = false;
	filter = CharFilterUnicode;
	convert = &NoConvert();
	inactive_convert = NULL;
	initcaps = false;
	maxlen = INT_MAX;
	autosize = false;
	keep_selection = false;
	errorbg = nobg = false;
	charset = CHARSET_UNICODE;
	alignright = false;
	SetStyle(StyleDefault());
	SetFrame(edge);
	font = StdFont();
}

EditField& EditField::SetFont(Font _font)
{
	font = _font;
	Finish(true);
	return *this;
}

EditField& EditField::NullText(const Image& icon, const char *text, Font fnt, Color ink)
{
	nullicon = icon;
	nulltext = text;
	nulltext << " ";
	nullink = ink;
	nullfont = fnt;
	Refresh();
	return *this;
}

EditField& EditField::NullText(const Image& icon, const char *text, Color ink)
{
	return NullText(icon, text, GetFont().Italic(), ink);
}

EditField& EditField::NullText(const char *text, Font fnt, Color ink)
{
	return NullText(Null, text, fnt, ink);
}

EditField& EditField::NullText(const char *text, Color ink)
{
	return NullText(text, GetFont().Italic(), ink);
}

EditField::EditField()
{
	dropcaret = Rect(0, 0, 0, 0);
	Unicode();
	Reset();
	WhenBar = THISBACK(StdBar);
}

EditField::~EditField() {}

void EditIntSpin::Inc()
{
	if(IsReadOnly()) return;
	int i = GetData();
	if(IsNull(i)) {
		if(IsNull(GetMin()) || GetMin() == INT_MIN) return;
		SetData(GetMin());
	}
	else
	if(!IsNull(GetMax()) && i < GetMax())
		SetData(min(i + inc, GetMax()));
	else
		return;
	SetFocus();
	SetSelection();
	UpdateAction();
}

void EditIntSpin::Dec()
{
	if(IsReadOnly()) return;
	int i = GetData();
	if(IsNull(i)) {
		if(IsNull(GetMax()) || GetMax() == INT_MAX) return;
		SetData(GetMax());
	}
	else
	if(!IsNull(GetMin()) && i > GetMin())
		SetData(max(i - inc, GetMin()));
	else
		return;
	SetFocus();
	SetSelection();
	UpdateAction();
}

void EditIntSpin::Init()
{
	sb.inc.WhenAction = sb.inc.WhenRepeat = callback(this, &EditIntSpin::Inc);
	sb.dec.WhenAction = sb.dec.WhenRepeat = callback(this, &EditIntSpin::Dec);
	AddFrame(sb);
	inc = 1;
}

EditIntSpin::EditIntSpin()
{
	Init();
}

EditIntSpin::EditIntSpin(int min, int max)
{
	MinMax(min, max);
	Init();
}

EditIntSpin::~EditIntSpin() {}

bool EditIntSpin::Key(dword key, int repcnt)
{
	if(key == K_UP)   { Inc(); return true; }
	if(key == K_DOWN) { Dec(); return true; }
	return EditInt::Key(key, repcnt);
}

void EditIntSpin::MouseWheel(Point, int zdelta, dword)
{
	if(zdelta < 0)
		Dec();
	else
		Inc();
}

EditDoubleSpin::EditDoubleSpin(double inc) : inc(inc) { Init(); }
EditDoubleSpin::EditDoubleSpin(double min, double max, double inc) : EditDouble(min, max), inc(inc) { Init(); }

EditDoubleSpin::~EditDoubleSpin() {}

void EditDoubleSpin::Init()
{
	AddFrame(spin);
	spin.inc.WhenRepeat = spin.inc.WhenAction = THISBACK(Inc);
	spin.dec.WhenRepeat = spin.dec.WhenAction = THISBACK(Dec);
}

void EditDoubleSpin::Inc()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	double d = GetData();
	if(!IsNull(d))
	{
		d = (floor(d / inc + 1e-3) + 1) * inc;
		if(IsNull(GetMax()) || d <= GetMax())
		{
			SetData(d);
			Action();
		}
	}
	else if(minval != DOUBLE_NULL_LIM)
		SetData(minval);
	SetFocus();
}

void EditDoubleSpin::Dec()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	double d = GetData();
	if(!IsNull(d))
	{
		d = (ceil(d / inc - 1e-3) - 1) * inc;
		if(IsNull(GetMin()) || d >= GetMin())
		{
			SetData(d);
			Action();
		}
	}
	else if(maxval != -DOUBLE_NULL_LIM)
		SetData(maxval);
	SetFocus();
}

bool EditDoubleSpin::Key(dword key, int repcnt)
{
	if(key == K_UP)   { Inc(); return true; }
	if(key == K_DOWN) { Dec(); return true; }
	return EditDouble::Key(key, repcnt);
}

void EditDoubleSpin::MouseWheel(Point, int zdelta, dword)
{
	if(zdelta < 0)
		Dec();
	else
		Inc();
}

END_UPP_NAMESPACE
