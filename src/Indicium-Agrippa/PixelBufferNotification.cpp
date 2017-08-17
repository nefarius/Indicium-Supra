#include "PixelBufferNotification.h"


PixelBufferNotification::PixelBufferNotification(RGBQUAD* pixelBuffer, int width, int height, int x, int y) : _x(x), _y(y), _width(width), _height(height)
{
    _length = _width*_height;
    _pixelBuffer = new RGBQUAD[_length];

    memcpy_s(_pixelBuffer, _length * sizeof(RGBQUAD), pixelBuffer, _length * sizeof(RGBQUAD));

    uint32_t* dPtr = reinterpret_cast<uint32_t*>(_pixelBuffer);
    for (size_t count = 0; count < _width * _height * sizeof(RGBQUAD); count += sizeof(RGBQUAD))
    {
        uint32_t t = *(dPtr);
        uint32_t t1 = (t & 0x00ff0000) >> 16;
        uint32_t t2 = (t & 0x000000ff) << 16;
        uint32_t t3 = (t & 0x0000ff00);
        uint32_t ta = (t & 0xFF000000);

        *(dPtr++) = t1 | t2 | t3 | ta;
    }
}

PixelBufferNotification::~PixelBufferNotification()
{
    if (_pixelBuffer)
        delete[] _pixelBuffer;
}
