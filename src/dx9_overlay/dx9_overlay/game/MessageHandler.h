#pragma once
#include <utils/BitStream.h>
#include <shared/PipeMessages.h>

#include <boost/bind.hpp>


void TextCreate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextDestroy(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextSetShadow(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextSetShown(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextSetColor(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextSetPos(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextSetString(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void TextUpdate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);

void BoxCreate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxDestroy(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetShown(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetBorder(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetBorderColor(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetColor(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetHeight(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetPos(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void BoxSetWidth(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);

void LineCreate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void LineDestroy(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void LineSetShown(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void LineSetColor(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void LineSetWidth(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void LineSetPos(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);

void ImageCreate(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ImageDestroy(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ImageSetShown(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ImageSetAlign(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ImageSetPos(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ImageSetRotation(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);

void DestroyAllVisual(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void ShowAllVisual(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);
void HideAllVisual(PipeMessages::ePipeMessages, BitStream *bsIn, BitStream *bsOut);