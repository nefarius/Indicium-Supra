#pragma once

#include <Poco/Runnable.h>
#include <Poco/NotificationQueue.h>

using Poco::NotificationQueue;

class IPCServer :
    public Poco::Runnable
{
    void* _context;
    void* _responder;
    NotificationQueue& _queue;

public:
    explicit IPCServer(NotificationQueue& queue);
    ~IPCServer();

    void run() override;
};

