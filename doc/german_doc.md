.TH "overlay.h" 3 "Sam Mai 17 2014" "DX9 Overlay API (de)" \" -*- nroff -*-
.ad l
.nh
.SH NAME
overlay.h \- 
.SH SYNOPSIS
.br
.PP
.SS "Funktionen"

.in +1c
.ti -1c
.RI "int \fBTextCreate\fP (const char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, const char *text, bool bShadow, bool bShow)"
.br
.ti -1c
.RI "int \fBTextDestroy\fP (int ID)"
.br
.ti -1c
.RI "int \fBTextSetShadow\fP (int id, bool b)"
.br
.ti -1c
.RI "int \fBTextSetShown\fP (int id, bool b)"
.br
.ti -1c
.RI "int \fBTextSetColor\fP (int id, unsigned int color)"
.br
.ti -1c
.RI "int \fBTextSetPos\fP (int id, int x, int y)"
.br
.ti -1c
.RI "int \fBTextSetString\fP (int id, const char *str)"
.br
.ti -1c
.RI "int \fBTextUpdate\fP (int id, const char *Font, int FontSize, bool bBold, bool bItalic)"
.br
.ti -1c
.RI "int \fBBoxCreate\fP (int x, int y, int w, int h, unsigned int dwColor, bool bShow)"
.br
.ti -1c
.RI "int \fBBoxDestroy\fP (int id)"
.br
.ti -1c
.RI "int \fBBoxSetShown\fP (int id, bool bShown)"
.br
.ti -1c
.RI "int \fBBoxSetBorder\fP (int id, int height, bool bShown)"
.br
.ti -1c
.RI "int \fBBoxSetBorderColor\fP (int id, unsigned int dwColor)"
.br
.ti -1c
.RI "int \fBBoxSetColor\fP (int id, unsigned int dwColor)"
.br
.ti -1c
.RI "int \fBBoxSetHeight\fP (int id, int height)"
.br
.ti -1c
.RI "int \fBBoxSetPos\fP (int id, int x, int y)"
.br
.ti -1c
.RI "int \fBBoxSetWidth\fP (int id, int width)"
.br
.ti -1c
.RI "int \fBLineCreate\fP (int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow)"
.br
.ti -1c
.RI "int \fBLineDestroy\fP (int id)"
.br
.ti -1c
.RI "int \fBLineSetShown\fP (int id, bool bShown)"
.br
.ti -1c
.RI "int \fBLineSetColor\fP (int id, unsigned int color)"
.br
.ti -1c
.RI "int \fBLineSetWidth\fP (int id, int width)"
.br
.ti -1c
.RI "int \fBLineSetPos\fP (int id, int x1, int y1, int x2, int y2)"
.br
.ti -1c
.RI "int \fBImageCreate\fP (const char *path, int x, int y, int rotation, int align, bool bShow)"
.br
.ti -1c
.RI "int \fBImageDestroy\fP (int id)"
.br
.ti -1c
.RI "int \fBImageSetShown\fP (int id, bool bShown)"
.br
.ti -1c
.RI "int \fBImageSetAlign\fP (int id, int align)"
.br
.ti -1c
.RI "int \fBImageSetPos\fP (int id, int x, int y)"
.br
.ti -1c
.RI "int \fBImageSetRotation\fP (int id, int rotation)"
.br
.ti -1c
.RI "int \fBDestroyAllVisual\fP ()"
.br
.ti -1c
.RI "int \fBShowAllVisual\fP ()"
.br
.ti -1c
.RI "int \fBHideAllVisual\fP ()"
.br
.ti -1c
.RI "int \fBInit\fP ()"
.br
.ti -1c
.RI "void \fBSetParam\fP (const char *_szParamName, const char *_szParamValue)"
.br
.in -1c
.SH "Dokumentation der Funktionen"
.PP 
.SS "int BoxCreate (intx, inty, intw, inth, unsigned intdwColor, boolbShow)"

.SS "int BoxDestroy (intid)"

.SS "int BoxSetBorder (intid, intheight, boolbShown)"

.SS "int BoxSetBorderColor (intid, unsigned intdwColor)"

.SS "int BoxSetColor (intid, unsigned intdwColor)"

.SS "int BoxSetHeight (intid, intheight)"

.SS "int BoxSetPos (intid, intx, inty)"

.SS "int BoxSetShown (intid, boolbShown)"

.SS "int BoxSetWidth (intid, intwidth)"

.SS "int DestroyAllVisual ()"

.SS "int HideAllVisual ()"

.SS "int ImageCreate (const char *path, intx, inty, introtation, intalign, boolbShow)"

.SS "int ImageDestroy (intid)"

.SS "int ImageSetAlign (intid, intalign)"

.SS "int ImageSetPos (intid, intx, inty)"

.SS "int ImageSetRotation (intid, introtation)"

.SS "int ImageSetShown (intid, boolbShown)"

.SS "int Init ()"

.SS "int LineCreate (intx1, inty1, intx2, inty2, intwidth, unsigned intcolor, boolbShow)"

.SS "int LineDestroy (intid)"

.SS "int LineSetColor (intid, unsigned intcolor)"

.SS "int LineSetPos (intid, intx1, inty1, intx2, inty2)"

.SS "int LineSetShown (intid, boolbShown)"

.SS "int LineSetWidth (intid, intwidth)"

.SS "void SetParam (const char *_szParamName, const char *_szParamValue)"

.SS "int ShowAllVisual ()"

.SS "int TextCreate (const char *Font, intFontSize, boolbBold, boolbItalic, intx, inty, unsigned intcolor, const char *text, boolbShadow, boolbShow)"
Erstellt einen Text mit den angegebenen Parametern\&. 
.PP
\fBParameter:\fP
.RS 4
\fIFont\fP Name einer Font 
.br
\fIFontSize\fP Größe der Font 
.br
\fIbBold\fP Sollte die Font fett sein? 
.br
\fIbItalic\fP Sollte die Font kursiv sein? 
.br
\fIx\fP Position X skaliert auf einer Auflösung von 800 x 600 
.br
\fIy\fP Position Y skaliert auf einer Auflösung von 800 x 600 
.br
\fIcolor\fP Farbe des Overlays (ARGB-Code) 
.br
\fItext\fP Text des Overlays 
.br
\fIbShadow\fP Schwarzer Schatten um das Overlay? 
.br
\fIbShow\fP Sollte der Text nach dem Funktionsaufruf gezeichnet werden? 
.RE
.PP
\fBRückgabe:\fP
.RS 4
Gibt die ID des Overlays zurück, welche für alle weitern Funktionen benötigt wird\&. Gibt -1 bei einem Fehler zurück\&. 
.RE
.PP

.SS "int TextDestroy (intID)"
Zerstört einen Text\&. 
.PP
\fBParameter:\fP
.RS 4
\fIID\fP TextID welche zerstört werden sollte\&. 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextSetColor (intid, unsigned intcolor)"
Setzt eine neue Farbe für den Text 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIcolor\fP Die neue Farbe als ARGB-Code 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextSetPos (intid, intx, inty)"
Setzt den Text an eine angegebene Position 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIx\fP X-Position skaliert auf einer Auflösung von 800 x 600 
.br
\fIy\fP Y-Position skaliert auf einer Auflösung von 800 x 600 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextSetShadow (intid, boolb)"
Aktiviert / Deaktiviert den Schatten eines Textes\&. 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIb\fP true für das Aktivieren, false für das Deaktivieren 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextSetShown (intid, boolb)"
Aktiviert / Deaktiviert das Overlay temporär\&. 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIb\fP true für das Aktivieren, false für das Deaktivieren 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextSetString (intid, const char *str)"
Setzen eines neues Textes für das Text-Overlay\&. 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIstr\fP Neuer Text als String 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SS "int TextUpdate (intid, const char *Font, intFontSize, boolbBold, boolbItalic)"
Aktualisieren des Overlays mit neuen Daten\&. 
.PP
\fBParameter:\fP
.RS 4
\fIid\fP Overlay-ID 
.br
\fIFont\fP Neue Font als String 
.br
\fIFontSize\fP Größe der Font 
.br
\fIbBold\fP Sollte der Text fett sein? 
.br
\fIbItalic\fP Sollte der Text kursiv sein? 
.RE
.PP
\fBRückgabe:\fP
.RS 4
1 wenn erfolgreich, 0 wenn nicht\&. 
.RE
.PP
\fBSiehe auch:\fP
.RS 4
\fBTextCreate()\fP 
.RE
.PP

.SH "Autor"
.PP 
Automatisch erzeugt von Doxygen für DX9 Overlay API (de) aus dem Quellcode\&.
