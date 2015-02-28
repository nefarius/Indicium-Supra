#include "Render.h"

#include <Utils/Misc.h>
#include <Utils/Bitstream.h>
#include <Utils/PipeClient.h>
#include <Utils/Windows.h>
#include <Shared/PipeMessages.h>

#include <boost/filesystem.hpp>

#define BITSTREAM_RET(T) { T retVal; bsOut >> retVal; return retVal; }

EXPORT int TextCreate(char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, char *text, bool bShadow, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextCreate << std::string(Font) << FontSize << bBold << bItalic << x << y << color << std::string(text);
	bsIn << bShadow << bShow;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return -1;
}

EXPORT int TextDestroy(int Id)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextDestroy << Id;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextSetShadow(int id, bool b)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextSetShadow << id << b;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextSetShown(int id, bool b)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextSetShown << id << b;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextSetColor << id << color;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextSetPos << id << x << y;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextSetString(int id, char *str)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextSetString << id << std::string(str);

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int TextUpdate(int id, char *Font, int FontSize, bool bBold, bool bItalic)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::TextUpdate << id << std::string(Font) << FontSize << bBold << bItalic;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxCreate << x << y << w << h << dwColor << bShow;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return -1;
}

EXPORT int BoxDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxDestroy << id;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetShown << id << bShown;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetBorder(int id, int height, bool bShown)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetBorder << id << height << bShown;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetBorderColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetBorderColor << id << dwColor;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetColor << id << dwColor;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetHeight(int id, int height)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetHeight << id << height;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetPos << id << x << y;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int BoxSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::BoxSetWidth << id << width;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineCreate << x1 << y1 << x2 << y2 << width << color << bShow;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return -1;
}

EXPORT int LineDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineDestroy << id;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int LineSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineSetShown << id << bShown;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int LineSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineSetColor << id << color;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int LineSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineSetWidth << id << width;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);
	
	return 0;
}

EXPORT int LineSetPos(int id, int x1, int y1, int x2, int y2)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::LineSetPos << id << x1 << y1 << x2 << y2;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int ImageCreate(char *path, int x, int y, int rotation, int align, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer bsIn, bsOut;

	std::string abs_path = boost::filesystem::absolute(path).string();
	if (!boost::filesystem::exists(abs_path))
		return -2;

	bsIn << PipeMessages::ImageCreate << abs_path << x << y << rotation << align << bShow;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return -1;
}

EXPORT int ImageDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ImageDestroy << id;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);
	
	return 0;
}

EXPORT int ImageSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ImageSetShown << id << bShown;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int ImageSetAlign(int id, int align)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ImageSetAlign << id << align;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int ImageSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ImageSetPos << id << x << y;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return 0;
}

EXPORT int ImageSetRotation(int id, int rotation)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ImageSetRotation << id << rotation;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);
	
	return 0;
}

EXPORT int DestroyAllVisual()
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::DestroyAllVisual;

	if (PipeClient(bsIn, bsOut).success())
		return 1;

	return 0;
}

EXPORT int ShowAllVisual()
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::ShowAllVisual;

	if (PipeClient(bsIn, bsOut).success())
		return 1;

	return 0;
}

EXPORT int HideAllVisual()
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::HideAllVisual;

	if (PipeClient(bsIn, bsOut).success())
		return 1;

	return 0;
}

EXPORT int GetFrameRate()
{
	SERVER_CHECK(-1)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::GetFrameRate;

	if (PipeClient(bsIn, bsOut).success())
		BITSTREAM_RET(int);

	return -1;
}

EXPORT int GetScreenSpecs(int& width, int& height)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::GetScreenSpecs;

	if (PipeClient(bsIn, bsOut).success())
	{
		bsOut >> width >> height;
		return 1;
	}

	return 0;
}

EXPORT int SetCalculationRatio(int width, int height)
{
	SERVER_CHECK(0)

	Serializer bsIn, bsOut;

	bsIn << PipeMessages::SetCalculationRatio << width << height;

	return (int)PipeClient(bsIn, bsOut).success();
}