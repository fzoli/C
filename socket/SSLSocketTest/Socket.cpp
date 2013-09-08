/* 
 * File:   Socket.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 5., 18:55
 */

#include "Socket.h"
#include "SocketException.h"
#include "SocketBuffer.h"

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sstream>

Socket::Socket() : closed(false) {
    buffer = NULL;
    m_sock = 0;
}

Socket::~Socket() {
    if (buffer != NULL) delete buffer;
}

Socket::Socket(int sock) : closed(false) {
    buffer = new SocketBuffer(this);
    m_sock = sock;
}

Socket::Socket(const char *host, uint16_t port, int timeout) : closed(false) {
    buffer = new SocketBuffer(this);
    m_sock = tcpConnect(host, port, timeout);
}

bool Socket::isClosed() {
    return closed;
}

std::streambuf* Socket::getBuffer() {
    return buffer;
}

void Socket::close() {
    if (closed) return;
    closed = true;
    if (m_sock) ::close(m_sock);
}

int Socket::write(const char *text) const {
    return write(text, strlen(text));
}

void Socket::write(int byte) {
    if (byte < 0 || byte > 255) throw SocketException( "Byte out of range" );
    char unsigned byte_char = byte;
    write(&byte_char, 1);
}

int Socket::write(const void *buf, int num) const {
    int status = ::write(m_sock, buf, num);
    if (status <= 0)
        throw SocketException( "Could not write byte" );
    return status;
}

int Socket::read() {
    char buf[1];
    int status = read(&buf, 1);
    return status == 0 ? -1 : buf[0];
}

int Socket::read(void *buf, int num) const {
    int status = ::recv(m_sock, buf, num, 0);
    if (status < 0) throw SocketException ( "Could not read from socket." );
    return status;
}

void Socket::read(std::string& s) const {    
    int status, bufsize = 1000;
    char buf[bufsize + 1];
    
    std::ostringstream ss;
    do {
        status = read(buf, bufsize);
        ss.write(buf, status);
    } while (status > 0);
    
    s = ss.str().c_str();
    ss.clear();
}

int Socket::tcpConnect(const char *addr, uint16_t port, int timeout) {
    struct hostent *host = gethostbyname(addr);
    int handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle == -1) {
        throw SocketException ( "Could not create socket." );
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server.sin_zero), 8);

    int ret = 0;
    fd_set writefds;
    if (timeout > 0) {
        FD_ZERO(&writefds);
        FD_SET(handle, &writefds);
        timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        ret = select(handle+1, NULL, &writefds, NULL, &tv);
    }
    
    if (timeout <= 0 || (ret > 0 && FD_ISSET(handle, &writefds))) {
        int err; socklen_t size = sizeof(err);
        if ((connect(handle, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1) || getsockopt(handle, SOL_SOCKET, SO_ERROR, &err, &size) != 0) {
            throw SocketException ( "Could not connect to the server." );
        }
    }
    else {
        throw SocketException ( "Connect timeout error" );
    }
    
    return handle;
}

void Socket::setTimeout(int sec) {
    setTimeout(m_sock, sec);
}

void Socket::setTimeout(int sock, int sec) {
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(struct timeval));
}

const Socket& Socket::operator >> (std::string& s) const {
    read(s);
    return *this;
}

const Socket& Socket::operator << (const std::string& s) const {
    write(s.c_str());
    return *this;
}
