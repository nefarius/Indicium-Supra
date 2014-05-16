#include "MessageHandler.h"
#include "game.h"
#include "rendering/Box.h"
#include "rendering/Line.h"
#include "rendering/Text.h"
#include "rendering/Renderer.h"
#include "rendering/RenderBase.h"
#include "rendering/Text.h"
#include "rendering/Line.h"
#include "rendering/Box.h"
#include "rendering/Image.h"


void TextCreate(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	std::string Font, Text;
	bool bBold, bItalic;
	int x, y, FontSize;
	unsigned int color;
	bool bShadow, bShow;

	bsIn->Read(Font);
	bsIn->Read(FontSize);
	bsIn->Read(bBold);
	bsIn->Read(bItalic);
	bsIn->Read(x);
	bsIn->Read(y);
	bsIn->Read(color);
	bsIn->Read(Text);
	bsIn->Read(bShadow);
	bsIn->Read(bShow);

	boost::shared_ptr<CText> obj(new CText(&g_renderer, Font.c_str(), FontSize, bBold, bItalic, x, y, color, Text.c_str(), bShadow, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void TextDestroy(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bsIn->Read(id);

	bsOut->Write((int) g_renderer.Remove(id));
}

void TextSetShadow(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bool bShadow;

	bsIn->Read(id);
	bsIn->Read(bShadow);

	boost::shared_ptr<CText> obj = g_renderer.Get<CText>(id);
	if (obj)
	{
		obj->SetShadow(bShadow);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetShown(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bool bShown;

	bsIn->Read(id);
	bsIn->Read(bShown);


	boost::shared_ptr<CText> text = g_renderer.Get<CText>(id);
	if (text)
	{
		text->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetColor(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	unsigned int color;

	bsIn->Read(id);
	bsIn->Read(color);


	boost::shared_ptr<CText> text = g_renderer.Get<CText>(id);
	if (text)
	{
		text->SetColor(color);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetPos(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	int x, y;

	bsIn->Read(id);
	bsIn->Read(x);
	bsIn->Read(y);


	boost::shared_ptr<CText> text = g_renderer.Get<CText>(id);
	if (text)
	{
		text->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetString(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	std::string str;
	bsIn->Read(id);
	bsIn->Read(str);

	boost::shared_ptr<CText> text = g_renderer.Get<CText>(id);
	if (text)
	{
		text->SetText(str.c_str());
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextUpdate(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	std::string Font;
	int len;
	int FontSize;
	bool bBold;
	bool bItalic;

	bsIn->Read(id);
	bsIn->Read(len);
	bsIn->Read(Font);
	bsIn->Read(FontSize);
	bsIn->Read(bBold);
	bsIn->Read(bItalic);


	boost::shared_ptr<CText> text = g_renderer.Get<CText>(id);
	if (text)
		bsOut->Write((int) text->UpdateText(Font.c_str(), FontSize, bBold, bItalic));
	else
		bsOut->Write((int) 0);
}

void BoxCreate(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int x, y, w, h;
	unsigned int dwColor;
	bool bShow;

	bsIn->Read(x);
	bsIn->Read(y);
	bsIn->Read(w);
	bsIn->Read(h);
	bsIn->Read(dwColor);
	bsIn->Read(bShow);

	boost::shared_ptr<CBox> obj(new CBox(&g_renderer, x, y, w, h, dwColor, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void BoxDestroy(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bsIn->Read(id);

	bsOut->Write((int) g_renderer.Remove(id));
}

void BoxSetShown(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bool bShown;

	bsIn->Read(id);
	bsIn->Read(bShown);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetBorder(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id, height;
	bool bShown;

	bsIn->Read(id);
	bsIn->Read(height);
	bsIn->Read(bShown);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetBorderWidth(height);
		box->SetBorderShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetBorderColor(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	unsigned int dwColor;

	bsIn->Read(id);
	bsIn->Read(dwColor);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetBorderColor(dwColor);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetColor(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	unsigned int dwColor;

	bsIn->Read(id);
	bsIn->Read(dwColor);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetBoxColor(dwColor);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetHeight(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id, height;

	bsIn->Read(id);
	bsIn->Read(height);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetBoxHeight(height);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetPos(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id, x, y;

	bsIn->Read(id);
	bsIn->Read(x);
	bsIn->Read(y);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetWidth(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id, width;

	bsIn->Read(id);
	bsIn->Read(width);


	boost::shared_ptr<CBox> box = g_renderer.Get<CBox>(id);
	if (box)
	{
		box->SetBoxWidth(width);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineCreate(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int x1; int y1; int x2; int y2; int width; unsigned int color; bool bShow;

	bsIn->Read(x1);
	bsIn->Read(y1);
	bsIn->Read(x2);
	bsIn->Read(y2);
	bsIn->Read(width);
	bsIn->Read(color);
	bsIn->Read(bShow);

	boost::shared_ptr<CLine> obj(new CLine(&g_renderer, x1, y1, x2, y2, width, color, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void LineDestroy(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bsIn->Read(id);

	bsOut->Write((int) g_renderer.Remove(id));
}

void LineSetShown(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id; bool bShown;
	bsIn->Read(id);
	bsIn->Read(bShown);


	boost::shared_ptr<CLine> line = g_renderer.Get<CLine>(id);
	if (line)
	{
		line->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetColor(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id; unsigned int color;
	bsIn->Read(id);
	bsIn->Read(color);


	boost::shared_ptr<CLine> line = g_renderer.Get<CLine>(id);
	if (line)
	{
		line->SetColor(color);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetWidth(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id; int width;
	bsIn->Read(id);
	bsIn->Read(width);


	boost::shared_ptr<CLine> line = g_renderer.Get<CLine>(id);
	if (line)
	{
		line->SetWidth(width);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetPos(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	int id; int x1; int y1; int x2; int y2;
	bsIn->Read(id);
	bsIn->Read(x1);
	bsIn->Read(y1);
	bsIn->Read(x2);
	bsIn->Read(y2);


	boost::shared_ptr<CLine> line = g_renderer.Get<CLine>(id);
	if (line)
	{
		line->SetPos(x1, y1, x2, y2);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void ImageCreate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	std::string path;
	int x, y, rotation, align;
	bool show;

	bsIn->Read(path);
	bsIn->Read(x);
	bsIn->Read(y);
	bsIn->Read(rotation);
	bsIn->Read(align);
	bsIn->Read(show);

	boost::shared_ptr<CImage> obj(new CImage(&g_renderer, path.c_str(), x, y, rotation, align, show));

	bsOut->Write(g_renderer.Add(obj));
}

void ImageDestroy(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bsIn->Read(id);

	bsOut->Write((int) g_renderer.Remove(id));
}

void ImageSetShown(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	bool bShow;

	bsIn->Read(id);
	bsIn->Read(bShow);


	boost::shared_ptr<CImage> obj = g_renderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetShown(bShow);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetAlign(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	int align;

	bsIn->Read(id);
	bsIn->Read(align);


	boost::shared_ptr<CImage> obj = g_renderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetAlign(align);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetPos(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	int x, y;

	bsIn->Read(id);
	bsIn->Read(x);
	bsIn->Read(y);


	boost::shared_ptr<CImage> obj = g_renderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetRotation(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut)
{
	int id;
	int rotation;

	bsIn->Read(id);
	bsIn->Read(rotation);


	boost::shared_ptr<CImage> obj = g_renderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetRotation(rotation);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}


void DestroyAllVisual(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	g_renderer.DestroyAll();
}

void ShowAllVisual(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	g_renderer.ShowAll();
}

void HideAllVisual(PipeMessages::ePipeMessages _id, BitStream *bsIn, BitStream *bsOut)
{
	g_renderer.HideAll();
}