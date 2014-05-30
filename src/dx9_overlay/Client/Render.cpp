#include "Render.h"

#include <Utils/Misc.h>
#include <Utils/Bitstream.h>
#include <Utils/NamedPipeClient.h>
#include <Utils/Windows.h>
#include <Shared/PipeMessages.h>

#include <boost/filesystem.hpp>

EXPORT int TextCreate(char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, char *text, bool bShadow, bool bShow)
{
	SERVER_CHECK(-1)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextCreate << std::string(Font) << FontSize << bBold << bItalic << x << y << color << std::string(text);
	bsIn << bShadow << bShow;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return -1;
}

EXPORT int TextDestroy(int Id)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextDestroy << Id;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextSetShadow(int id, bool b)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextSetShadow << id << b;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextSetShown(int id, bool b)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextSetShown << id << b;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextSetColor << id << color;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextSetPos << id << x << y;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextSetString(int id, char *str)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextSetString << id << std::string(str);

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int TextUpdate(int id, char *Font, int FontSize, bool bBold, bool bItalic)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::TextUpdate << id << std::string(Font) << FontSize << bBold << bItalic;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow)
{
	SERVER_CHECK(-1)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxCreate << x << y << w << h << dwColor << bShow;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return -1;
}

EXPORT int BoxDestroy(int id)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxDestroy << id;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetShown << id << bShown;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetBorder(int id, int height, bool bShown)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetBorder << id << height << bShown;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetBorderColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetBorderColor << id << dwColor;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetColor << id << dwColor;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetHeight(int id, int height)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetHeight << id << height;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetPos << id << x << y;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int BoxSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::BoxSetWidth << id << width;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow)
{
	SERVER_CHECK(-1)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineCreate << x1 << y1 << x2 << y2 << width << color << bShow;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return -1;
}

EXPORT int LineDestroy(int id)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineDestroy << id;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int LineSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineSetShown << id << bShown;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int LineSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineSetColor << id << color;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int LineSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineSetWidth << id << width;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}
	
	return 0;
}

EXPORT int LineSetPos(int id, int x1, int y1, int x2, int y2)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::LineSetPos << id << x1 << y1 << x2 << y2;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int ImageCreate(char *path, int x, int y, int rotation, int align, bool bShow)
{
	SERVER_CHECK(-1)

	CBitStream bsIn, bsOut;

	std::string abs_path = boost::filesystem::absolute(path).string();
	if (!boost::filesystem::exists(abs_path))
		return -2;

	bsIn << (short) PipeMessages::ImageCreate << abs_path << x << y << rotation << align << bShow;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return -1;
}

EXPORT int ImageDestroy(int id)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ImageDestroy << id;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}
	
	return 0;
}

EXPORT int ImageSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ImageSetShown << id << bShown;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int ImageSetAlign(int id, int align)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ImageSetAlign << id << align;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int ImageSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ImageSetPos << id << x << y;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return 0;
}

EXPORT int ImageSetRotation(int id, int rotation)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ImageSetRotation << id << rotation;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}
	
	return 0;
}

EXPORT int DestroyAllVisual()
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::DestroyAllVisual;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
		return 1;

	return 0;
}

EXPORT int ShowAllVisual()
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::ShowAllVisual;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
		return 1;

	return 0;
}

EXPORT int HideAllVisual()
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::HideAllVisual;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
		return 1;

	return 0;
}

EXPORT int GetFrameRate()
{
	SERVER_CHECK(-1)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::GetFrameRate;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		int iRet;
		bsOut.Read(iRet);
		return iRet;
	}

	return -1;
}

EXPORT int GetScreenSpecs(int& width, int& height)
{
	SERVER_CHECK(0)

	CBitStream bsIn, bsOut;

	bsIn << (short) PipeMessages::GetScreenSpecs;

	if (CNamedPipeClient("Overlay_Server", &bsIn, &bsOut).Success())
	{
		bsOut >> width >> height;
		return 1;
	}

	return 0;
}