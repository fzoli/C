/* 
 * File:   ServerSocket.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 5., 20:04
 */

#include "ServerSocket.h"
#include "SocketException.h"

#include <unistd.h>

ServerSocket::ServerSocket() {
    ;
}

ServerSocket::ServerSocket(uint16_t port, uint16_t maxNewConn) {
    open(port, maxNewConn);
}

Socket* ServerSocket::accept() {
    return new Socket(tcpAccept());
}

bool ServerSocket::isClosed() {
    return closed;
}

void ServerSocket::close() {
    if (closed) return;
    closed = true;
    if (h.socket) ::close(h.socket);
}

void ServerSocket::open(uint16_t port, uint16_t maxConn) {
    int handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle == -1) {
        throw SocketException ( "Could not create server socket." );
    }
    else {
        // Sets REUSEADDR option to avoid socket bind after the application closes.
        const int       optVal = 1;
        const socklen_t optLen = sizeof(optVal);
        setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
        
        h.server.sin_family = AF_INET;
        h.server.sin_addr.s_addr = INADDR_ANY;
        h.server.sin_port = htons(port);
        int server_length = sizeof (h.server);
        if (bind(handle, (struct sockaddr *) &h.server, server_length) == -1) {
            throw SocketException ( "Could not bind to port." );
        }
        if (listen(handle, maxConn) == -1) {
            throw SocketException ( "Could not listen to socket." );
        }
        h.socket = handle;
        h.serverlen = server_length;
    }
}

int ServerSocket::tcpAccept() {
    int newhandle = ::accept(h.socket, (struct sockaddr *) &h.server, (socklen_t *) &h.serverlen);
    if (newhandle <= 0) {
        throw SocketException ( "Could not accept socket." );
    }
    if (timeout > 0) {
        Socket::setTimeout(newhandle, timeout);
    }
    return newhandle;
}

void ServerSocket::setTimeout(int sec) {
//    Socket::setTimeout(h.socket, sec); // do not need because has no effect
    timeout = sec;
}
