#include "RichText.h"

NAMESPACE_UPP

String HtmlFontStyle(Font f, Font base)
{
	String style;
	if(f.GetFace() != base.GetFace())
		switch(f.GetFace()) {
		case Font::ARIAL: style = "font-family:sans-serif;"; break;
		case Font::ROMAN: style = "font-family:serif;"; break;
		case Font::COURIER: style = "font-family:monospace;"; break;
		}
	if(f.GetHeight() != base.GetHeight())
		style << Sprintf("font-size:%dpt;", f.GetHeight() * 72 / 600);
	if(f.IsBold() != base.IsBold())
		style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	if(f.IsItalic() != base.IsItalic())
		style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	if(f.IsUnderline() != base.IsUnderline())
		style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
	return style;
}

String HtmlFontStyle(Font f)
{
	String style;
	switch(f.GetFace()) {
		case Font::ARIAL: style = "font-family:sans-serif;"; break;
		case Font::ROMAN: style = "font-family:serif;"; break;
		case Font::COURIER: style = "font-family:monospace;"; break;
	}
	style << Sprintf("font-size:%dpt;", f.GetHeight() * 72 / 600);
	style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
	return style;
}

String HtmlDot(int q, Zoom z)
{
	return String().Cat() << ' ' << z * q << "px";
}

String HtmlDotl(int q, Zoom z)
{
	return String().Cat() << ' ' << max((int)!!q, z * q) << "px";
}

String HtmlStyleColor(Color c, const char *cl = "color")
{
	return Format(String(cl) + ":#%02x%02x%02x;", c.GetR(), c.GetG(), c.GetB());
}

String HtmlCharStyle(const RichPara::CharFormat& cf, const RichPara::CharFormat& sf)
{
	String style;
	if(cf.ink != sf.ink)
		style = HtmlStyleColor(cf.ink);
	return style + HtmlFontStyle(cf, sf);
}

String HtmlParaStyle(const RichPara::Format& f, Zoom z)
{
	String style;
	int lm = z * f.lm;
	if(f.bullet && f.bullet != RichPara::BULLET_TEXT) {
		style << "display:list-item;line-style-type:";
		switch(f.bullet) {
		case RichPara::BULLET_ROUND: style << "disc"; break;
		case RichPara::BULLET_ROUNDWHITE: style << "circle"; break;
		case RichPara::BULLET_BOX:
		case RichPara::BULLET_BOXWHITE: style << "square"; break;
		}
		style << ";line-style-position:inside;";
		lm += 20;
	}
	style << Format("margin:%d %d %d %d;text-indent:%d;",
	                z * f.before,
	                z * f.rm,
	                z * f.after,
	                lm,
	                z * (f.bullet ? 0 : f.indent)
	         );
	style << "text-align:";
	switch(f.align) {
	case ALIGN_LEFT: style << "left;"; break;
	case ALIGN_RIGHT: style << "right;"; break;
	case ALIGN_CENTER: style << "center;"; break;
	case ALIGN_JUSTIFY: style << "justify;"; break;
	}
	style << HtmlStyleColor(f.ink) + HtmlFontStyle(f);
	if(!IsNull(f.paper))
		style << HtmlStyleColor(f.paper, "background-color");
	return style;
}

String FormatClass(Index<String>& css, const String& fmt)
{
	return " class=\"" + FormatIntAlpha(css.FindAdd(fmt) + 1) + "\"";
}

void TabBorder(String& style, const char *txt, int border, Color bordercolor, const RichTable::Format& tf, Zoom z)
{
	style << "border-" << txt << ':' << HtmlDotl(border + tf.grid, z) << " solid "
	      << ColorToHtml(border ? bordercolor : tf.gridcolor) << ';';
}


String AsHtml(const RichTxt& text, const RichStyles& styles, Index<String>& css,
              const VectorMap<String, String>& links,
              const VectorMap<String, String>& labels,
              const String& outdir, const String& namebase, Zoom z, int& im,
              const VectorMap<String, String>& escape,
              int imtolerance)
{
	String html;
	for(int i = 0; i < text.GetPartCount(); i++)
	{
		if(text.IsTable(i)) {
			const RichTable& t = text.GetTable(i);
			int nx = t.format.column.GetCount();
			int ny = t.cell.GetCount();
			const RichTable::Format& tf = t.format;
			html << "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
			if(tf.before > 0)
				html << "<tr><td height=" << HtmlDot(tf.before, z) << " colspan=\"3\"></td></tr>";
			html << "<tr>";
			if (tf.lm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.lm, z) << "><tr><td></td></tr></table></td>\r\n";
			html <<	"<td width=\"100%\">";

			String style;
			style << "border-collapse:collapse;table-layout:auto;"
			      << "border:" << HtmlDotl(tf.frame, z) << " solid " << ColorToHtml(tf.framecolor) << ';';

			html << "<table width=\"100%\"" << FormatClass(css, style) << ">";
			int sum = 0;
			for(int i = 0; i < nx; i++)
				sum += t.format.column[i];
			html << "<colgroup>";
			for(int i = 0; i < nx; i++)
				html << "<col width=\"" << 100 * t.format.column[i] / sum << "%\">";
			html << "</colgroup>";
			html << "\r\n";
			for(int i = 0; i < ny; i++) {
				const Array<RichCell>& r = t.cell[i];
				html << "<tr>";
				for(int j = 0; j < r.GetCount(); j++) {
					if(t(i, j)) {
						const RichCell& c = r[j];
						const RichCell::Format& cf = c.format;
						String style;
						style << "padding:" << HtmlDot(cf.margin.top, z) << HtmlDot(cf.margin.right, z)
						                    << HtmlDot(cf.margin.bottom, z) << HtmlDot(cf.margin.left, z) << ';';
						TabBorder(style, "left", cf.border.left, cf.bordercolor, tf, z);
						TabBorder(style, "top", cf.border.top, cf.bordercolor, tf, z);
						TabBorder(style, "right", cf.border.right, cf.bordercolor, tf, z);
						TabBorder(style, "bottom", cf.border.bottom, cf.bordercolor, tf, z);
						style << "background-color:" << ColorToHtml(cf.color) << ';';
						style << "vertical-align:";
						switch(cf.align) {
						case ALIGN_TOP:    style << "top"; break;
						case ALIGN_CENTER: style << "middle"; break;
						case ALIGN_BOTTOM: style << "bottom"; break;
						}
						style << ';';
						html << "<td" << FormatClass(css, style);
						if(c.hspan)
							html << " colspan=" << c.hspan + 1;
						if(c.vspan)
							html << " rowspan=" << c.vspan + 1;
						html << '>';
						html << AsHtml(c.text, styles, css, links, labels, outdir, namebase, z,
						               im, escape, imtolerance);
						html << "</td>\r\n";
					}
				}
				html << "</tr>\r\n";
			}
			html << "</table></td>\r\n";
			if (tf.rm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.rm, z) << "><tr><td></td></tr></table></td>";
			if(tf.after > 0)
				html << "<tr><td height=" << HtmlDot(tf.after, z) << " colspan=\"3\"></td></tr>";
			html << "</tr></table>\r\n";
		}
		else
		if(text.IsPara(i)) {
			RichPara p = text.Get(i, styles);
			if(p.format.ruler)
				html << "<hr>";
			String lbl;
			if(!IsNull(p.format.label)) {
				lbl = labels.Get(p.format.label, Null);
				if(lbl.GetCount())
					html << "<a name=\"" << lbl << "\">";
			}
			bool bultext = false;
			if(p.format.bullet == RichPara::BULLET_TEXT)
				for(int i = 0; i < p.part.GetCount(); i++) {
					const RichPara::Part& part = p.part[i];
					if(part.text.Find(9) >= 0) {
						bultext = true;
						break;
					}
				}
			if(bultext) {
				html << "<table width=\"100%\" border=\"0\" "
				        "cellpadding=\"2\" cellspacing=\"2\">"
				        "<tr>";
				int q = z * p.format.lm - 8;
				if(q > 0)
					html << Format("<td width=\"%d\"></td>", q);
				html << Format("<td valign=\"top\" width=\"%d\" bgcolor=\"#F0F0F0\">\r\n",
				               max(z * p.format.indent, 0));
				p.format.ruler = p.format.after = p.format.before = p.format.indent = p.format.lm = 0;
			}
			String par = "<p" + FormatClass(css, HtmlParaStyle(p.format, z)) + ">";
			html << par;
			for(int i = 0; i < p.part.GetCount(); i++) {
				const RichPara::Part& part = p.part[i];
				int q;
				if(part.object) {
					String name;
					name << namebase << "_" << im++ << ".png";
					Size psz = part.object.GetPixelSize();
					String lname;
					lname << "L$" << name;
					Size sz = z * part.object.GetSize();
					if(abs(100 * (psz.cx - sz.cx) / sz.cx) < imtolerance)
						sz = psz;
					PNGEncoder png;
					png.SaveFile(AppendFileName(outdir, name), part.object.ToImage(sz));
					if(psz.cx * psz.cy)
						html << "<a href=\"" << lname << "\">";
					html << "<img src=\"" << name << "\" border=\"0\" alt=\"\">";
					if(psz.cx * psz.cy) {
						html << "</a>";
						PNGEncoder png;
						png.SaveFile(AppendFileName(outdir, lname), part.object.ToImage(psz));
					}
				}
				else
				if(part.format.indexentry.GetCount() &&
				   (q = escape.Find(part.format.indexentry.ToString())) >= 0)
					html << escape[q];
				else {
					String lnk = part.format.link;
					if(lnk.GetCount()) {
						int q = links.Find(lnk);
						if(q < 0) {
							int q = lnk.ReverseFind('#');
							if(q >= 0) {
								String l = lnk.Left(q);
								lnk = links.Get(l, l) + '#' + lnk.Mid(q + 1);
							}
						}
						else
							lnk = links[q];
					}
					String endtag;
					if(!lnk.IsEmpty() && lnk[0] != ':') {
						html << "<a href=\"" << lnk << "\">";
						endtag = "</a>";
					}
					String cs;
					if(part.text[0] != 9)
						cs = HtmlCharStyle(part.format, p.format);
					if(!cs.IsEmpty()) {
						html << "<span" << FormatClass(css, cs) << ">";
						endtag = "</span>" + endtag;
					}
					if(part.format.sscript == 1) {
						html << "<sup>";
						endtag = "</sup>" + endtag;
					}
					if(part.format.sscript == 2) {
						html << "<sub>";
						endtag = "</sub>" + endtag;
					}
					if(part.format.IsStrikeout()) {
						html << "<strike>";
						endtag = "</strike>" + endtag;
					}
					if(part.format.capitals) {
						html << "<span style=\"font-variant: small-caps;\">";
						endtag << "</span>";
					}
					bool spc = false;
					const wchar *end = part.text.End();
					for(const wchar *s = part.text.Begin(); s != end; s++) {
						if(*s == ' ') {
							html.Cat(spc ? "&nbsp;" : " ");
							spc = true;
						}
						else {
							spc = false;
							if(*s == 160)  html.Cat("&nbsp;");
							else
							if(*s == '<')  html.Cat("&lt;");
							else
							if(*s == '>')  html.Cat("&gt;");
							else
							if(*s == '&')  html.Cat("&amp;");
							else
							if(*s == '\"') html.Cat("&quot;");
							else
							if(*s == 9) {
								if(bultext) {
									if(!cs.IsEmpty() && part.text[0] != 9)
										html << "</span>";
									html << "</p>";
									html << "</td>\r\n<td valign=\"top\" bgcolor=\"#F0F0F0\">\r\n";
									html << par;
									if(s[1]) {
										cs = HtmlCharStyle(part.format, p.format);
										if(!cs.IsEmpty())
											html << "<span" << FormatClass(css, cs) << ">";
									}
								}
								else
									html.Cat("&nbsp;&nbsp;&nbsp;&nbsp;");
							}
							else
								html.Cat(ToUtf8(*s));
						}
					}
					html << endtag;
				}
			}
			if(p.part.GetCount() == 0)
				html << "&nbsp;";
			html << "</p>";
			if(bultext)
				html << "</td></tr></table>";
			if(lbl.GetCount())
				html << "</a>";
			html << "\r\n";
		}
	}
	return html;
}

String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  const String& outdir, const String& namebase, Zoom z,
                  const VectorMap<String, String>& escape, int imt)
{
	int im = 0;
	return AsHtml(text, text.GetStyles(), css, links, labels, outdir, namebase, z, im, escape, imt);
}

String AsCss(Index<String>& ss)
{
	String css;
	for(int i = 0; i < ss.GetCount(); i++) {
		css << "." + FormatIntAlpha(i + 1);
		css << "{" << ss[i] << "}\r\n";
	}
	return css;
}

END_UPP_NAMESPACE
