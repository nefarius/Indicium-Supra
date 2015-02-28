#pragma once
#include <Utils/Bitstream.h>
#include <Shared/PipeMessages.h>

#include <boost/bind.hpp>

void TextCreate(CSerializer& bsIn, CSerializer& bsOut);
void TextDestroy(CSerializer& bsIn, CSerializer& bsOut);
void TextSetShadow(CSerializer& bsIn, CSerializer& bsOut);
void TextSetShown(CSerializer& bsIn, CSerializer& bsOut);
void TextSetColor(CSerializer& bsIn, CSerializer& bsOut);
void TextSetPos(CSerializer& bsIn, CSerializer& bsOut);
void TextSetString(CSerializer& bsIn, CSerializer& bsOut);
void TextUpdate(CSerializer& bsIn, CSerializer& bsOut);

void BoxCreate(CSerializer& bsIn, CSerializer& bsOut);
void BoxDestroy(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetShown(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetBorder(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetBorderColor(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetColor(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetHeight(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetPos(CSerializer& bsIn, CSerializer& bsOut);
void BoxSetWidth(CSerializer& bsIn, CSerializer& bsOut);

void LineCreate(CSerializer& bsIn, CSerializer& bsOut);
void LineDestroy(CSerializer& bsIn, CSerializer& bsOut);
void LineSetShown(CSerializer& bsIn, CSerializer& bsOut);
void LineSetColor(CSerializer& bsIn, CSerializer& bsOut);
void LineSetWidth(CSerializer& bsIn, CSerializer& bsOut);
void LineSetPos(CSerializer& bsIn, CSerializer& bsOut);

void ImageCreate(CSerializer& bsIn, CSerializer& bsOut);
void ImageDestroy(CSerializer& bsIn, CSerializer& bsOut);
void ImageSetShown(CSerializer& bsIn, CSerializer& bsOut);
void ImageSetAlign(CSerializer& bsIn, CSerializer& bsOut);
void ImageSetPos(CSerializer& bsIn, CSerializer& bsOut);
void ImageSetRotation(CSerializer& bsIn, CSerializer& bsOut);

void DestroyAllVisual(CSerializer& bsIn, CSerializer& bsOut);
void ShowAllVisual(CSerializer& bsIn, CSerializer& bsOut);
void HideAllVisual(CSerializer& bsIn, CSerializer& bsOut);

void GetFrameRate(CSerializer& bsIn, CSerializer& bsOut);
void GetScreenSpecs(CSerializer& bsIn, CSerializer& bsOut);