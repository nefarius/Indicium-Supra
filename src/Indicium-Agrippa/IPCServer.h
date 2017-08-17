#pragma once

#include <Poco/Runnable.h>
#include <Poco/NotificationQueue.h>
#define ZMQ_STATIC
#include <zmq.hpp>

using Poco::NotificationQueue;


class IPCServer :
    public Poco::Runnable
{
    NotificationQueue& _queue;
    zmq::context_t _context;
    zmq::socket_t _socket;

public:
    explicit IPCServer(NotificationQueue& queue);
    ~IPCServer() {}

    void run() override;
};

