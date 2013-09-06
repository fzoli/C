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

SSLServerSocket::SSLServerSocket(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify) {
    init(port, maxNewConn, CAfile, CRTfile, KEYfile, passwd, verify);
}

SSLServerSocket::SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify) {
    init(port, 10, CAfile, CRTfile, KEYfile, passwd, verify);
}

SSLServerSocket::SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, bool verify) {
    init(port, 10, CAfile, CRTfile, KEYfile, NULL, verify);
}

void SSLServerSocket::init(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify) {
    ctx = sslCreateCtx(false, verify, CAfile, CRTfile, KEYfile, passwd);
    open(port, maxNewConn);
}

bool SSLServerSocket::isClosed() {
    return ServerSocket::isClosed();
}

void SSLServerSocket::close() {
    if (isClosed()) return;
    ServerSocket::close();
    sslDestroyCtx(ctx);
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

void SSLServerSocket::setTimeout(int sec) {
    ServerSocket::setTimeout(sec);
}
