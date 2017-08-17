#include "IPCServer.h"
#include <Poco/Logger.h>
#include "PixelBufferNotification.h"

using Poco::Logger;


IPCServer::IPCServer(NotificationQueue& queue) : _queue(queue), _context(1), _socket(_context, ZMQ_REP)
{
    std::string bindAddr = "tcp://127.0.0.1:36129";
    auto& logger = Logger::get(typeid(*this).name());

    _socket.bind(bindAddr);

    logger.information("Binding IPCServer to %s", bindAddr);
}

void IPCServer::run()
{
    auto& logger = Logger::get(typeid(*this).name());

    logger.information("Message listener thread running");

    // TODO: this is messy and missing error handling
    while (true)
    {
        zmq::message_t x, y, width, height, buffer;

        _socket.recv(&x);
        _socket.recv(&y);
        _socket.recv(&width);
        _socket.recv(&height);
        _socket.recv(&buffer);

        logger.information("Buffer size: %s", std::to_string(buffer.size()));

        _queue.enqueueNotification(new PixelBufferNotification(
            static_cast<RGBQUAD*>(buffer.data()),
            std::stoi(static_cast<char*>(width.data())),
            std::stoi(static_cast<char*>(height.data())),
            std::stoi(static_cast<char*>(x.data())),
            std::stoi(static_cast<char*>(y.data()))));
                
        zmq::message_t reply(2);
        memcpy(reply.data(), "OK", 2);
        _socket.send(reply);
    }
}
