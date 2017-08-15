#include "IPCServer.h"
#define ZMQ_STATIC
#include "zmq.h"
#include <cassert>


IPCServer::IPCServer(NotificationQueue& queue) : _context(nullptr), _responder(nullptr), _queue(queue)
{
    _context = zmq_ctx_new();
    _responder = zmq_socket(_context, ZMQ_PAIR);

    auto rc = zmq_bind(_responder, "tcp://127.0.0.1:19622");
    assert(rc == 0);
}


IPCServer::~IPCServer()
{
    if (_context)
        zmq_ctx_destroy(_context);
}

void IPCServer::run()
{
    int64_t more;
    size_t more_size = sizeof more;

    while (true)
    {
        do {
            /* Create an empty ØMQ message to hold the message part */
            zmq_msg_t part;
            int rc = zmq_msg_init(&part);
            assert(rc == 0);
            /* Block until a message is available to be received from socket */
            rc = zmq_recvmsg(_responder, &part, 0);
            assert(rc == 0);
            /* Determine if more message parts are to follow */
            rc = zmq_getsockopt(_responder, ZMQ_RCVMORE, &more, &more_size);
            assert(rc == 0);
            zmq_msg_close(&part);
        } while (more);
    }
}
