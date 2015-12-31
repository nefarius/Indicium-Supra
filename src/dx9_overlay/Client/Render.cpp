#include "Render.h"

#include <Utils/Serializer.h>
#include <Utils/PipeClient.h>
#include <Utils/Windows.h>
#include <Shared/PipeMessages.h>

#include <boost/filesystem.hpp>

#define SERIALIZER_RET(T) { T retVal; serializerOut >> retVal; return retVal; }

EXPORT int TextCreate(char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, char *text, bool bShadow, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextCreate << std::string(Font) << FontSize << bBold << bItalic << x << y << color << std::string(text);
	serializerIn << bShadow << bShow;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return -1;
}

EXPORT int TextDestroy(int Id)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextDestroy << Id;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextSetShadow(int id, bool b)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextSetShadow << id << b;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextSetShown(int id, bool b)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextSetShown << id << b;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextSetColor << id << color;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextSetPos << id << x << y;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextSetString(int id, char *str)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextSetString << id << std::string(str);

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int TextUpdate(int id, char *Font, int FontSize, bool bBold, bool bItalic)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::TextUpdate << id << std::string(Font) << FontSize << bBold << bItalic;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxCreate << x << y << w << h << dwColor << bShow;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return -1;
}

EXPORT int BoxDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxDestroy << id;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetShown << id << bShown;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetBorder(int id, int height, bool bShown)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetBorder << id << height << bShown;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetBorderColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetBorderColor << id << dwColor;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetColor(int id, unsigned int dwColor)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetColor << id << dwColor;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetHeight(int id, int height)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetHeight << id << height;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetPos << id << x << y;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int BoxSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::BoxSetWidth << id << width;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineCreate << x1 << y1 << x2 << y2 << width << color << bShow;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return -1;
}

EXPORT int LineDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineDestroy << id;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int LineSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineSetShown << id << bShown;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int LineSetColor(int id, unsigned int color)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineSetColor << id << color;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int LineSetWidth(int id, int width)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineSetWidth << id << width;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);
	
	return 0;
}

EXPORT int LineSetPos(int id, int x1, int y1, int x2, int y2)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::LineSetPos << id << x1 << y1 << x2 << y2;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int ImageCreate(char *path, int x, int y, float scaleX, float scaleY, int rotation, int align, bool bShow)
{
	SERVER_CHECK(-1)

	Serializer serializerIn, serializerOut;

	std::string abs_path = boost::filesystem::absolute(path).string();
	if (!boost::filesystem::exists(abs_path))
		return -2;

	serializerIn << PipeMessages::ImageCreate << abs_path << x << y << scaleX << scaleY << rotation << align << bShow;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return -1;
}

EXPORT int ImageDestroy(int id)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageDestroy << id;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);
	
	return 0;
}

EXPORT int ImageSetShown(int id, bool bShown)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageSetShown << id << bShown;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int ImageSetAlign(int id, int align)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageSetAlign << id << align;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int ImageSetPos(int id, int x, int y)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageSetPos << id << x << y;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int ImageSetRotation(int id, int rotation)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageSetRotation << id << rotation;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);
	
	return 0;
}

EXPORT int ImageSetScale(int id, float x, float y)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ImageSetScale << id << x << y;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}

EXPORT int DestroyAllVisual()
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::DestroyAllVisual;

	if (PipeClient(serializerIn, serializerOut).success())
		return 1;

	return 0;
}

EXPORT int ShowAllVisual()
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::ShowAllVisual;

	if (PipeClient(serializerIn, serializerOut).success())
		return 1;

	return 0;
}

EXPORT int HideAllVisual()
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::HideAllVisual;

	if (PipeClient(serializerIn, serializerOut).success())
		return 1;

	return 0;
}

EXPORT int GetFrameRate()
{
	SERVER_CHECK(-1)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::GetFrameRate;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return -1;
}

EXPORT int GetScreenSpecs(int& width, int& height)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::GetScreenSpecs;

	if (PipeClient(serializerIn, serializerOut).success())
	{
		serializerOut >> width >> height;
		return 1;
	}

	return 0;
}

EXPORT int SetCalculationRatio(int width, int height)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::SetCalculationRatio << width << height;

	return (int)PipeClient(serializerIn, serializerOut).success();
}

EXPORT int SetOverlayPriority(int id, int priority)
{
	SERVER_CHECK(0)

	Serializer serializerIn, serializerOut;

	serializerIn << PipeMessages::SetOverlayPriority << id << priority;

	if (PipeClient(serializerIn, serializerOut).success())
		SERIALIZER_RET(int);

	return 0;
}


