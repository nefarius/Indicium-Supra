#include "IPCServer.h"
#define ZMQ_STATIC
#include "zmq.h"
#include <cassert>
#include <Poco/Logger.h>

using Poco::Logger;


IPCServer::IPCServer(NotificationQueue& queue) : _context(nullptr), _responder(nullptr), _queue(queue)
{
    std::string bindAddr = "tcp://127.0.0.1:5556";
    auto& logger = Logger::get(__func__);

    _context = zmq_ctx_new();
    _responder = zmq_socket(_context, ZMQ_REP);

    logger.information("Binding IPCServer to %s", bindAddr);

    auto rc = zmq_bind(_responder, bindAddr.c_str());
    
    if (rc != 0)
    {
        logger.fatal("Failed to bind IPCServer to %s: %s", bindAddr, std::to_string(GetLastError()));
    }
}


IPCServer::~IPCServer()
{
    if (_context)
        zmq_ctx_destroy(_context);
}

void IPCServer::run()
{
    auto& logger = Logger::get(__func__);

    while (true)
    {
        /* Create an empty ØMQ message to hold the message part */
        zmq_msg_t input;
        auto rc = zmq_msg_init(&input);

        if (rc != 0)
        {
            logger.error("zmq_msg_init failed: %s", std::to_string(rc));
        }

        /* Block until a message is available to be received from socket */
        rc = zmq_recvmsg(_responder, &input, 0);

        if (rc == -1)
        {
            logger.error("zmq_recvmsg failed: %s", std::to_string(rc));
        }

        logger.information("Received incoming message");

        zmq_msg_close(&input);

        zmq_msg_t output;
        zmq_msg_init_data(&output, "OK", 2, nullptr, nullptr);
        zmq_sendmsg(_responder, &output, 0);
        zmq_msg_close(&output);
    }
}
