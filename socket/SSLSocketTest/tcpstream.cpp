// Copyright (C) Tane Pendragon 2010
// Released under the Creative Commons Attribution License (cc by)

// tcpstream.cpp - implementation of the tcpstream class

#include <stdint.h>
#include <memory.h>
#include <fcntl.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "tcpstream.h"

const int BUFFER_SIZE = 1024;

struct datachunk {
    int bytes;
    uint8_t *data;
    datachunk *next;  // pointer to next datachunk, or NULL if this is the last

    datachunk(int size) : bytes(size), data(new uint8_t[size]), next(NULL) { }
    ~datachunk() { delete[] data; }
};

struct tcpbuf_data {
    std::string error;               // string description of last error
    std::string remotehost;          // human readable name of the remote host
    int socket;                      // identifier of open socket, or -1
    bool blocking;                   // true if socket has been set to block
    datachunk *inbox;                // pointer to the first unused data chunk
    uint8_t buffer[BUFFER_SIZE];

    tcpbuf_data() : socket(-1), blocking(true), inbox(NULL) { }
};

struct tcplistener_data {
    int backlog;
    int socket;

    tcplistener_data(int bklog) : backlog(bklog), socket(-1) { }
};

//----------------------------------------------------------------------
// tcpbuf - a std::streambuf for a TCP stream

// construction / destruction
tcpbuf::tcpbuf() {
    m_data = new tcpbuf_data();
    m_data->inbox = new datachunk(1);
    char_type *data = (char_type *)m_data->inbox->data;
    setg(data, data, data);
}

tcpbuf::~tcpbuf() {
    close();
    // free up any buffered data
    datachunk *curchunk = m_data->inbox;
    while (curchunk != NULL) {
        datachunk *old = curchunk;
        curchunk = curchunk->next;
        delete old;
    }
    delete m_data;
    m_data = NULL;
}

// set the buffer to block, or not, on socket input
void tcpbuf::blocking(bool block) {
    m_data->blocking = block;
    if (m_data->socket) {
        if (!block) {
            int flags = fcntl(m_data->socket, F_GETFL, 0);
            fcntl(m_data->socket, F_SETFL, flags & ~O_NONBLOCK);
        }
        else fcntl(m_data->socket, F_SETFL, O_NONBLOCK);
    }
}

// connect to the given host and port number
bool tcpbuf::connect(const std::string& host, const std::string& port) {
    m_data->error == "";

    // attempt to resolve the hostname
    addrinfo hints, *info, *cur;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo(host.c_str(), port.c_str(), &hints, &info);
    if (ret != 0) {
        m_data->error = "Couldn't resolve address: ";
        m_data->error = m_data->error + gai_strerror(ret);
        return false;
    }

    // try to connect to the host
    for (cur = info; cur != NULL && m_data->socket == -1; cur = cur->ai_next) {
        m_data->socket = socket(cur->ai_family,
            cur->ai_socktype, cur->ai_protocol);
        if (m_data->socket != -1) {
            // we can bind via this protocol, can we connect?
            if (::connect(m_data->socket, cur->ai_addr, cur->ai_addrlen) == -1) {
                ::close(m_data->socket);
                m_data->socket = -1;
            } else {
                m_data->remotehost = host + ":" + port;
            }
        }
    }
    freeaddrinfo(info);

    // if the connection failed, make a note
    if (m_data->socket == -1)
        m_data->error = "Couldn't connect to " + host + ":" + port;

    return m_data->socket != -1;
}

// returns true if this socket is connected
bool tcpbuf::connected() {
    return m_data->socket != -1;
}

// returns true if there is a complete line in the inbox
bool tcpbuf::hasline() {
    fillbuffer();
    // search buffer for a newline
    int startpos = gptr() - eback();
    datachunk *curchunk = m_data->inbox;
    while (curchunk != NULL) {
        for (int i=startpos; i<curchunk->bytes; i++) {
            if (curchunk->data[i] == '\n')
                return true;
        }
        curchunk = curchunk->next;
        startpos = 0;
    }
    return false;
}

// returns the number of bytes of data waiting to be read
int tcpbuf::hasdata() {
    fillbuffer();
    // count the total amount of data in the inbox
    int count = 0;
    datachunk *curchunk = m_data->inbox;
    while (curchunk != NULL) {
        count += curchunk->bytes;
        curchunk = curchunk->next;
    }
    return count;
}

// get the last error to occur
const std::string& tcpbuf::error() {
    return m_data->error;
}

// get the name of the remote host
const std::string& tcpbuf::remotehost() {
    return m_data->remotehost;
}

// closes the connection and resets the stream
void tcpbuf::close() {
    if (m_data->socket != -1)
        ::close(m_data->socket);
    m_data->socket = -1;
}

// implementation of underflow() for std::streambuf
int tcpbuf::underflow() {
    // check for more data; if we're a blocking socket and we don't have another
    // buffer waiting then block until more data arrives
    fillbuffer(m_data->inbox->next == NULL);

    // bail if we don't have another buffer to swap to yet
    if (m_data->inbox == NULL || m_data->inbox->next == NULL) return EOF;

    // we've used up the buffer at the front of the inbox, so free it
    datachunk *old = m_data->inbox;
    m_data->inbox = old->next;
    delete old;

    // and set the new buffer up for the stream to read from
    char_type *newdata = (char_type *)m_data->inbox->data;
    setg(newdata, newdata, newdata + m_data->inbox->bytes);
    return *newdata;
}

// implementation of overflow() for std::streambuf
int tcpbuf::overflow(int c) {
    if (m_data->socket == -1) return -1;
    return write(m_data->socket, &c, sizeof(c));
}

// implementation of xsputn() for std::streambuf
std::streamsize tcpbuf::xsputn(const char *s, std::streamsize n) {
    if (m_data->socket == -1) return -1;
    return write(m_data->socket, s, n);
}

// implementation of xsgetn() for std::streambuf
std::streamsize tcpbuf::xsgetn(char *s, std::streamsize n) {
    if (m_data->socket == -1) return -1;
    return recv(m_data->socket, s, n, 0);
}

// tops up the buffer with any data waiting in the socket
bool tcpbuf::fillbuffer(bool block) {
    if (m_data->socket == -1) return false; // not connected

    if (!block || !m_data->blocking) {
        // bail if there's no data in the socket waiting to be read
        timeval waittime = { 0, 0 };
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(m_data->socket, &readset);
        select(m_data->socket+1, &readset, NULL, NULL, &waittime);
        if (!FD_ISSET(m_data->socket, &readset))
            return false; // no data
    }

    // read the available data...
    int num = recv(m_data->socket, m_data->buffer, BUFFER_SIZE, 0);
    if (num <= 0) {
        m_data->error = "Connection lost.";
        close();
        return false;
    }

    // ...and append it to the inbox
    datachunk *curchunk = m_data->inbox, *newchunk = new datachunk(num);
    memcpy(newchunk->data, m_data->buffer, num);
    if (curchunk != NULL) {
        while (curchunk->next != NULL)
            curchunk = curchunk->next;
        curchunk->next = newchunk;
    } else {
        m_data->inbox = newchunk;
    }
    return true;
}

//----------------------------------------------------------------------
// tcplistener - a class which listens on a port and accepts connections

tcplistener::tcplistener(int backlog) {
    m_data = new tcplistener_data(backlog);
}

tcplistener::~tcplistener() {
    close();
    delete m_data;
    m_data = NULL;
}

// listen on the given port for incoming socket connections
bool tcplistener::listen(const std::string& port) {
    if (m_data->socket != -1) return false; // already listening

    // bind to the requested port
    addrinfo hints, *info, *cur;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int r;
    if ((r = getaddrinfo(NULL, port.c_str(), &hints, &info)) != 0) return false;

    // try to get a socket to bind to the port
    for (cur = info; cur != NULL && m_data->socket == -1; cur = cur->ai_next) {
        m_data->socket = socket(cur->ai_family,
            cur->ai_socktype, cur->ai_protocol);
        if (m_data->socket != -1) {
            // insert lame joke about rings here
            if (bind(m_data->socket, cur->ai_addr, cur->ai_addrlen) == -1)
                close();
        }
    }
    freeaddrinfo(info);

    // if we have a socket, listen on it
    if (m_data->socket != -1 && ::listen(m_data->socket, m_data->backlog) == -1)
        close();

    return m_data->socket != -1;
}

// returns true if the listener is listening
bool tcplistener::listening() {
    return m_data->socket != -1;
}

// returns true if any incoming connections are pending
bool tcplistener::pending() {
    timeval waittime = { 0, 0 };
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(m_data->socket, &readset);
    select(m_data->socket+1, &readset, NULL, NULL, &waittime);
    return FD_ISSET(m_data->socket, &readset);
}

// accept an incoming socket connection on the supplied socket
// if no connection is pending, this will block until a connection is made
bool tcplistener::accept(tcpstream& sock) {
    // make sure sock isn't already connected
    if (sock.m_data->socket != -1) return false;

    // accept the incoming connection
    sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    sock.m_data->socket = ::accept(m_data->socket, (sockaddr *)&addr, &addrlen);
    if (sock.m_data->socket == -1) return false; // fail :(

    // tell the socket where it came from
    void *in_addr;
    if (((sockaddr *)&addr)->sa_family == AF_INET) // IPv4
        in_addr = &(((sockaddr_in *)&addr)->sin_addr);
    else
        in_addr = &(((sockaddr_in6 *)&addr)->sin6_addr);
    char ip[INET6_ADDRSTRLEN];
    sock.m_data->remotehost = inet_ntop(addr.ss_family,
        in_addr, ip, INET6_ADDRSTRLEN);

    // winnar!
    return true;
}

// stop listening for connections
void tcplistener::close() {
    if (m_data->socket != -1)
        ::close(m_data->socket);
    m_data->socket = -1;
}
