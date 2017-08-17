#include "PixelBufferNotification.h"



PixelBufferNotification::PixelBufferNotification(RGBQUAD* pixelBuffer, int width, int height, int x, int y) : _x(x), _y(y), _width(width), _height(height)
{
    _length = _width*_height;
    _pixelBuffer = new RGBQUAD[_length];

    memcpy_s(_pixelBuffer, _length, pixelBuffer, _length);
}


PixelBufferNotification::~PixelBufferNotification()
{
    if (_pixelBuffer)
        delete[] _pixelBuffer;
}
