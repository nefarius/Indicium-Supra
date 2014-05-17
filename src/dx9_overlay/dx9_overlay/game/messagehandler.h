#pragma once
#include <utils/bitstream.h>
#include <shared/pipemessages.h>

#include <boost/bind.hpp>


void TextCreate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextDestroy(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextSetShadow(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextSetShown(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextSetColor(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextSetPos(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextSetString(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void TextUpdate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);

void BoxCreate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxDestroy(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetShown(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetBorder(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetBorderColor(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetColor(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetHeight(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetPos(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void BoxSetWidth(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);

void LineCreate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void LineDestroy(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void LineSetShown(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void LineSetColor(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void LineSetWidth(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void LineSetPos(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);

void ImageCreate(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ImageDestroy(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ImageSetShown(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ImageSetAlign(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ImageSetPos(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ImageSetRotation(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);

void DestroyAllVisual(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void ShowAllVisual(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);
void HideAllVisual(PipeMessages::ePipeMessages, bitstream *bsIn, bitstream *bsOut);