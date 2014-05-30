#pragma once
#include <Utils/Bitstream.h>
#include <Shared/PipeMessages.h>

#include <boost/bind.hpp>

void TextCreate(CBitStream& bsIn, CBitStream& bsOut);
void TextDestroy(CBitStream& bsIn, CBitStream& bsOut);
void TextSetShadow(CBitStream& bsIn, CBitStream& bsOut);
void TextSetShown(CBitStream& bsIn, CBitStream& bsOut);
void TextSetColor(CBitStream& bsIn, CBitStream& bsOut);
void TextSetPos(CBitStream& bsIn, CBitStream& bsOut);
void TextSetString(CBitStream& bsIn, CBitStream& bsOut);
void TextUpdate(CBitStream& bsIn, CBitStream& bsOut);

void BoxCreate(CBitStream& bsIn, CBitStream& bsOut);
void BoxDestroy(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetShown(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetBorder(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetBorderColor(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetColor(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetHeight(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetPos(CBitStream& bsIn, CBitStream& bsOut);
void BoxSetWidth(CBitStream& bsIn, CBitStream& bsOut);

void LineCreate(CBitStream& bsIn, CBitStream& bsOut);
void LineDestroy(CBitStream& bsIn, CBitStream& bsOut);
void LineSetShown(CBitStream& bsIn, CBitStream& bsOut);
void LineSetColor(CBitStream& bsIn, CBitStream& bsOut);
void LineSetWidth(CBitStream& bsIn, CBitStream& bsOut);
void LineSetPos(CBitStream& bsIn, CBitStream& bsOut);

void ImageCreate(CBitStream& bsIn, CBitStream& bsOut);
void ImageDestroy(CBitStream& bsIn, CBitStream& bsOut);
void ImageSetShown(CBitStream& bsIn, CBitStream& bsOut);
void ImageSetAlign(CBitStream& bsIn, CBitStream& bsOut);
void ImageSetPos(CBitStream& bsIn, CBitStream& bsOut);
void ImageSetRotation(CBitStream& bsIn, CBitStream& bsOut);

void DestroyAllVisual(CBitStream& bsIn, CBitStream& bsOut);
void ShowAllVisual(CBitStream& bsIn, CBitStream& bsOut);
void HideAllVisual(CBitStream& bsIn, CBitStream& bsOut);

void GetFrameRate(CBitStream& bsIn, CBitStream& bsOut);
void GetScreenSpecs(CBitStream& bsIn, CBitStream& bsOut);