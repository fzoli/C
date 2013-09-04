/* 
 * File:   SSLServerSocket.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 2., 20:29
 */

#include "SSLServerSocket.h"
#include "SSLSocketException.h"

#include <unistd.h>

#include <openssl/err.h>

SSLServerSocket::SSLServerSocket(uint16_t port, int maxConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd) {
    ctx = sslCreateCtx(false, CAfile, CRTfile, KEYfile, passwd);
    open(port, maxConn);
}

void SSLServerSocket::open(uint16_t port, int maxConn) {
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

void SSLServerSocket::close() {
    if (closed) return;
    closed = true;
    if (h.socket) ::close(h.socket);
    sslDestroyCtx(ctx);
}

int SSLServerSocket::tcpAccept() {
    int newhandle = ::accept(h.socket, (struct sockaddr *) &h.server, (socklen_t *) &h.serverlen);
    if (newhandle <= 0) {
        throw SocketException ( "Could not accept socket." );
    }
    return newhandle;
}

SSLSocket::connection SSLServerSocket::sslAccept() {
    connection c;
    c.sslHandle = NULL;
    c.socket = tcpAccept();
    
    // Create an SSL struct for the connection
    c.sslHandle = SSL_new(ctx);
    if (c.sslHandle == NULL)
        throw SSLSocketException ( "Could not create SSL object." );

    // Connect the SSL struct to our connection
    if (!SSL_set_fd(c.sslHandle, c.socket))
        throw SSLSocketException ( "Could not connect the SSL object to the socket." );

    // Initiate SSL handshake
    if (SSL_accept(c.sslHandle) != 1)
        throw SSLSocketException ( "Error during SSL handshake." );
        
    return c;
}

SSLSocket SSLServerSocket::accept() {
    connection c = sslAccept();
    SSLSocket s = SSLSocket(c);
    return s;
}
