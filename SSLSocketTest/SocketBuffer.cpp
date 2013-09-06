/* 
 * File:   SSLStream.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 4., 18:55
 */

#include "SocketBuffer.h"
#include "SocketException.h"

#include <string.h>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

struct sb_datachunk {
    int bytes;
    uint8_t *data;
    sb_datachunk *next;  // pointer to next datachunk, or NULL if this is the last

    sb_datachunk(int size) : bytes(size), data(new uint8_t[size]), next(NULL) { }
    ~sb_datachunk() { delete[] data; }
};

struct sb_data {
    sb_datachunk *inbox;                // pointer to the first unused data chunk
    uint8_t buffer[BUFFER_SIZE];

    sb_data() : inbox(NULL) { }
};

SocketBuffer::SocketBuffer(Socket* socket) {
    if (socket == NULL) throw SocketException("socket is null");
    this->socket = socket;
    m_data = new sb_data();
    m_data->inbox = new sb_datachunk(1);
    char_type *data = (char_type *)m_data->inbox->data;
    setg(data, data, data);
}

SocketBuffer::~SocketBuffer() {
    // free up any buffered data
    sb_datachunk *curchunk = m_data->inbox;
    while (curchunk != NULL) {
        sb_datachunk *old = curchunk;
        curchunk = curchunk->next;
        delete old;
    }
    delete m_data;
    m_data = NULL;
}

bool SocketBuffer::fillbuffer() {
    if (socket->isClosed()) return false; // not connected

    // read the available data...
    int num;
    try {
        num = socket->read(m_data->buffer, BUFFER_SIZE);
    }
    catch (SocketException ex) {
        num = -1;
    }
    
    // close the socket if no data available or exception has been throwed
    if (num <= 0) {
        socket->close();
        return false;
    }
    
    // ...and append it to the inbox
    sb_datachunk *curchunk = m_data->inbox, *newchunk = new sb_datachunk(num);
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

int SocketBuffer::underflow() {
    // check for more data; if we're a blocking socket and we don't have another
    // buffer waiting then block until more data arrives
    fillbuffer(/*m_data->inbox->next == NULL*/);

    // bail if we don't have another buffer to swap to yet
    if (m_data->inbox == NULL || m_data->inbox->next == NULL) {
        return EOF;
    }

    // we've used up the buffer at the front of the inbox, so free it
    sb_datachunk *old = m_data->inbox;
    m_data->inbox = old->next;
    delete old;

    // and set the new buffer up for the stream to read from
    char_type *newdata = (char_type *)m_data->inbox->data;
    setg(newdata, newdata, newdata + m_data->inbox->bytes);
    return *newdata;
}

int SocketBuffer::overflow(int c) {
    try {
        return socket->write(&c, sizeof(c));
    }
    catch (SocketException ex) {
        socket->close();
        return -1;
    }
}

std::streamsize SocketBuffer::xsputn(const char *s, std::streamsize n) {
    try {
        return socket->write(s, n);
    }
    catch (SocketException ex) {
        socket->close();
        return -1;
    }
}

std::streamsize SocketBuffer::xsgetn(char *s, std::streamsize n) {
    try {
        return socket->read(s, n);
    }
    catch (SocketException ex) {
        socket->close();
        return -1;
    }
}
