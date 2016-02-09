#pragma once
#include <Utils/Serializer.h>
#include <Shared/PipeMessages.h>

#include <functional>

void TextCreate(Serializer& serializerIn, Serializer& serializerOut);
void TextDestroy(Serializer& serializerIn, Serializer& serializerOut);
void TextSetShadow(Serializer& serializerIn, Serializer& serializerOut);
void TextSetShown(Serializer& serializerIn, Serializer& serializerOut);
void TextSetColor(Serializer& serializerIn, Serializer& serializerOut);
void TextSetPos(Serializer& serializerIn, Serializer& serializerOut);
void TextSetString(Serializer& serializerIn, Serializer& serializerOut);
void TextUpdate(Serializer& serializerIn, Serializer& serializerOut);

void BoxCreate(Serializer& serializerIn, Serializer& serializerOut);
void BoxDestroy(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetShown(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetBorder(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetBorderColor(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetColor(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetHeight(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetPos(Serializer& serializerIn, Serializer& serializerOut);
void BoxSetWidth(Serializer& serializerIn, Serializer& serializerOut);

void LineCreate(Serializer& serializerIn, Serializer& serializerOut);
void LineDestroy(Serializer& serializerIn, Serializer& serializerOut);
void LineSetShown(Serializer& serializerIn, Serializer& serializerOut);
void LineSetColor(Serializer& serializerIn, Serializer& serializerOut);
void LineSetWidth(Serializer& serializerIn, Serializer& serializerOut);
void LineSetPos(Serializer& serializerIn, Serializer& serializerOut);

void ImageCreate(Serializer& serializerIn, Serializer& serializerOut);
void ImageDestroy(Serializer& serializerIn, Serializer& serializerOut);
void ImageSetShown(Serializer& serializerIn, Serializer& serializerOut);
void ImageSetAlign(Serializer& serializerIn, Serializer& serializerOut);
void ImageSetPos(Serializer& serializerIn, Serializer& serializerOut);
void ImageSetRotation(Serializer& serializerIn, Serializer& serializerOut);
void ImageSetScale(Serializer& serializerIn, Serializer& serializerOut);

void DestroyAllVisual(Serializer& serializerIn, Serializer& serializerOut);
void ShowAllVisual(Serializer& serializerIn, Serializer& serializerOut);
void HideAllVisual(Serializer& serializerIn, Serializer& serializerOut);

void GetFrameRate(Serializer& serializerIn, Serializer& serializerOut);
void GetScreenSpecs(Serializer& serializerIn, Serializer& serializerOut);

void SetCalculationRatio(Serializer& serializerIn, Serializer& serializerOut);

void SetOverlayPriority(Serializer& serializerIn, Serializer& serializerOut);