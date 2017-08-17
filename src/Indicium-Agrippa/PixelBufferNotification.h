#pragma once
#include <Poco/Notification.h>

class PixelBufferNotification :
    public Poco::Notification
{
    size_t _length;
    int _x;
    int _y;
    int _width;
    int _height;
    RGBQUAD* _pixelBuffer;

public:
    PixelBufferNotification(RGBQUAD* pixelBuffer, int width, int height, int x, int y);
    ~PixelBufferNotification();

    size_t getLength() const
    {
        return _length;
    }

    int getX() const
    {
        return _x;
    }

    int getY() const
    {
        return _y;
    }

    int getWidth() const
    {
        return _width;
    }

    int getHeight() const
    {
        return _height;
    }

    RGBQUAD* getBuffer() const
    {
        return _pixelBuffer;
    }
};

