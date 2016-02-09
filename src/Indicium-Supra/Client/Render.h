#pragma once
#include "Client.h"

EXPORT int TextCreate(char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, char *text, bool bShadow, bool bShow);
EXPORT int TextDestroy(int ID);
EXPORT int TextSetShadow(int id, bool b);
EXPORT int TextSetShown(int id, bool b);
EXPORT int TextSetColor(int id, unsigned int color);
EXPORT int TextSetPos(int id, int x, int y);
EXPORT int TextSetString(int id, char *str);
EXPORT int TextUpdate(int id, char *Font, int FontSize, bool bBold, bool bItalic);

EXPORT int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow);
EXPORT int BoxDestroy(int id);
EXPORT int BoxSetShown(int id, bool bShown);
EXPORT int BoxSetBorder(int id, int height, bool bShown);
EXPORT int BoxSetBorderColor(int id, unsigned int dwColor);
EXPORT int BoxSetColor(int id, unsigned int dwColor);
EXPORT int BoxSetHeight(int id, int height);
EXPORT int BoxSetPos(int id, int x, int y);
EXPORT int BoxSetWidth(int id, int width);

EXPORT int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow);
EXPORT int LineDestroy(int id);
EXPORT int LineSetShown(int id, bool bShown);
EXPORT int LineSetColor(int id, unsigned int color);
EXPORT int LineSetWidth(int id, int width);
EXPORT int LineSetPos(int id, int x1, int y1, int x2, int y2);

EXPORT int ImageCreate(char *path, int x, int y, float scaleX, float scaleY, int rotation, int align, bool bShow);
EXPORT int ImageDestroy(int id);
EXPORT int ImageSetShown(int id, bool bShown);
EXPORT int ImageSetAlign(int id, int align);
EXPORT int ImageSetPos(int id, int x, int y);
EXPORT int ImageSetRotation(int id, int rotation);
EXPORT int ImageSetScale(int id, float x, float y);

EXPORT int DestroyAllVisual();
EXPORT int ShowAllVisual();
EXPORT int HideAllVisual();

EXPORT int GetFrameRate();
EXPORT int GetScreenSpecs(int& width, int& height);

EXPORT int SetCalculationRatio(int width, int height);
EXPORT int SetOverlayPriority(int id, int priority);