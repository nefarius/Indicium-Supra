#include "messagehandler.h"
#include "game.h"
#include "rendering/text.h"
#include "rendering/box.h"
#include "rendering/line.h"
#include "rendering/image.h"
#include "rendering/renderer.h"
#include "rendering/renderbase.h"


void TextCreate(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	std::string Font, Text; bool bBold, bItalic; int x, y, FontSize; unsigned int color; bool bShadow, bShow;

	*bsIn >> Font >> FontSize >> bBold >> bItalic >> x >> y >> color >> Text >> bShadow >> bShow;

	boost::shared_ptr<text> obj(new text(&g_renderer, Font.c_str(), FontSize, bBold, bItalic, x, y, color, Text.c_str(), bShadow, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void TextDestroy(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id;

	*bsIn >> id;

	bsOut->Write((int) g_renderer.Remove(id));
}

void TextSetShadow(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; bool bShadow;

	*bsIn >> id >> bShadow;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
	{
		obj->SetShadow(bShadow);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetShown(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; bool bShown;

	*bsIn >> id >> bShown;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetColor(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; unsigned int color;

	*bsIn >> id >> color;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
	{
		obj->SetColor(color);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetPos(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, x, y;

	*bsIn >> id >> x >> y;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextSetString(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; std::string str;

	*bsIn >> id >> str;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
	{
		obj->SetText(str.c_str());
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void TextUpdate(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, FontSize; std::string Font; bool bBold, bItalic;

	*bsIn >> id >> Font >> FontSize >> bBold >> bItalic;

	boost::shared_ptr<text> obj = g_renderer.Get<text>(id);
	if (obj)
		bsOut->Write((int) obj->UpdateText(Font.c_str(), FontSize, bBold, bItalic));
	else
		bsOut->Write((int) 0);
}

void BoxCreate(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int x, y, w, h; unsigned int dwColor; bool bShow;

	*bsIn >> x >> y >> w >> h >> dwColor >> bShow;

	boost::shared_ptr<box> obj(new box(&g_renderer, x, y, w, h, dwColor, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void BoxDestroy(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id;

	*bsIn >> id;

	bsOut->Write((int) g_renderer.Remove(id));
}

void BoxSetShown(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; bool bShown;

	*bsIn >> id >> bShown;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetBorder(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, height; bool bShown;

	*bsIn >> id >> height >> bShown;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetBorderWidth(height);
		obj->SetBorderShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetBorderColor(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; unsigned int dwColor;

	*bsIn >> id >> dwColor;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetBorderColor(dwColor);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetColor(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; unsigned int dwColor;

	*bsIn >> id >> dwColor;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetBoxColor(dwColor);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetHeight(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, height;

	*bsIn >> id >> height;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetBoxHeight(height);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetPos(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, x, y;

	*bsIn >> id >> x >> y;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void BoxSetWidth(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, width;

	*bsIn >> id >> width;

	boost::shared_ptr<box> obj = g_renderer.Get<box>(id);
	if (obj)
	{
		obj->SetBoxWidth(width);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineCreate(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int x1, y1, x2, y2, width; unsigned int color; bool bShow;

	*bsIn >> x1 >> y1 >> x2 >> y2 >> width >> color >> bShow;

	boost::shared_ptr<line> obj(new line(&g_renderer, x1, y1, x2, y2, width, color, bShow));

	bsOut->Write(g_renderer.Add(obj));
}

void LineDestroy(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id;

	*bsIn >> id;

	bsOut->Write((int) g_renderer.Remove(id));
}

void LineSetShown(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; bool bShown;

	*bsIn >> id >> bShown;

	boost::shared_ptr<line> obj = g_renderer.Get<line>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetColor(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; unsigned int color;

	*bsIn >> id >> color;

	boost::shared_ptr<line> obj = g_renderer.Get<line>(id);
	if (obj)
	{
		obj->SetColor(color);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetWidth(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id; int width;

	*bsIn >> id >> width;

	boost::shared_ptr<line> obj = g_renderer.Get<line>(id);
	if (obj)
	{
		obj->SetWidth(width);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void LineSetPos(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	int id, x1, y1, x2, y2;

	*bsIn >> id >> x1 >> y1 >> x2 >> y2;

	boost::shared_ptr<line> obj = g_renderer.Get<line>(id);
	if (obj)
	{
		obj->SetPos(x1, y1, x2, y2);
		bsOut->Write((int) 1);
	}
	else
		bsOut->Write((int) 0);
}

void ImageCreate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	std::string path; int x, y, rotation, align; bool show;

	*bsIn >> path >> x >> y >> rotation >> align >> show;

	boost::shared_ptr<image> obj(new image(&g_renderer, path.c_str(), x, y, rotation, align, show));

	bsOut->Write(g_renderer.Add(obj));
}

void ImageDestroy(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	int id;

	*bsIn >> id;

	bsOut->Write((int) g_renderer.Remove(id));
}

void ImageSetShown(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	int id; bool bShow;

	*bsIn >> id >> bShow;

	boost::shared_ptr<image> obj = g_renderer.Get<image>(id);
	if (obj)
	{
		obj->SetShown(bShow);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetAlign(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	int id, align;

	*bsIn >> id >> align;

	boost::shared_ptr<image> obj = g_renderer.Get<image>(id);
	if (obj)
	{
		obj->SetAlign(align);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetPos(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	int id, x, y;

	*bsIn >> id >> x >> y;

	boost::shared_ptr<image> obj = g_renderer.Get<image>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}

void ImageSetRotation(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	int id, rotation;

	*bsIn >> id >> rotation;

	boost::shared_ptr<image> obj = g_renderer.Get<image>(id);
	if (obj)
	{
		obj->SetRotation(rotation);
		bsOut->Write((int) 1);
	}
	bsOut->Write((int) 0);
}


void DestroyAllVisual(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	g_renderer.DestroyAll();
}

void ShowAllVisual(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	g_renderer.ShowAll();
}

void HideAllVisual(PipeMessages::ePipeMessages _id, bitstream *bsIn, bitstream *bsOut)
{
	g_renderer.HideAll();
}

void GetFrameRate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	*bsOut << g_renderer.GetFrameRate();
}

void GetScreenSpecs(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut)
{
	*bsOut << g_renderer.GetScreenWidth() << g_renderer.GetScreenHeight();
}