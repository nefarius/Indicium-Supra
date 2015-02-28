#pragma once
#include <Utils/Bitstream.h>
#include <Shared/PipeMessages.h>

#include <boost/bind.hpp>

void TextCreate(Serializer& bsIn, Serializer& bsOut);
void TextDestroy(Serializer& bsIn, Serializer& bsOut);
void TextSetShadow(Serializer& bsIn, Serializer& bsOut);
void TextSetShown(Serializer& bsIn, Serializer& bsOut);
void TextSetColor(Serializer& bsIn, Serializer& bsOut);
void TextSetPos(Serializer& bsIn, Serializer& bsOut);
void TextSetString(Serializer& bsIn, Serializer& bsOut);
void TextUpdate(Serializer& bsIn, Serializer& bsOut);

void BoxCreate(Serializer& bsIn, Serializer& bsOut);
void BoxDestroy(Serializer& bsIn, Serializer& bsOut);
void BoxSetShown(Serializer& bsIn, Serializer& bsOut);
void BoxSetBorder(Serializer& bsIn, Serializer& bsOut);
void BoxSetBorderColor(Serializer& bsIn, Serializer& bsOut);
void BoxSetColor(Serializer& bsIn, Serializer& bsOut);
void BoxSetHeight(Serializer& bsIn, Serializer& bsOut);
void BoxSetPos(Serializer& bsIn, Serializer& bsOut);
void BoxSetWidth(Serializer& bsIn, Serializer& bsOut);

void LineCreate(Serializer& bsIn, Serializer& bsOut);
void LineDestroy(Serializer& bsIn, Serializer& bsOut);
void LineSetShown(Serializer& bsIn, Serializer& bsOut);
void LineSetColor(Serializer& bsIn, Serializer& bsOut);
void LineSetWidth(Serializer& bsIn, Serializer& bsOut);
void LineSetPos(Serializer& bsIn, Serializer& bsOut);

void ImageCreate(Serializer& bsIn, Serializer& bsOut);
void ImageDestroy(Serializer& bsIn, Serializer& bsOut);
void ImageSetShown(Serializer& bsIn, Serializer& bsOut);
void ImageSetAlign(Serializer& bsIn, Serializer& bsOut);
void ImageSetPos(Serializer& bsIn, Serializer& bsOut);
void ImageSetRotation(Serializer& bsIn, Serializer& bsOut);

void DestroyAllVisual(Serializer& bsIn, Serializer& bsOut);
void ShowAllVisual(Serializer& bsIn, Serializer& bsOut);
void HideAllVisual(Serializer& bsIn, Serializer& bsOut);

void GetFrameRate(Serializer& bsIn, Serializer& bsOut);
void GetScreenSpecs(Serializer& bsIn, Serializer& bsOut);

void SetCalculationRatio(Serializer& bsIn, Serializer& bsOut);