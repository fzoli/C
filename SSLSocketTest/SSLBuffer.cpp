/* 
 * File:   SSLStream.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 4., 18:55
 */

#include "SSLBuffer.h"
#include "SSLSocketException.h"

const int BUFFER_SIZE = 1024;

struct ssldatachunk {
    int bytes;
    uint8_t *data;
    ssldatachunk *next;  // pointer to next datachunk, or NULL if this is the last

    ssldatachunk(int size) : bytes(size), data(new uint8_t[size]), next(NULL) { }
    ~ssldatachunk() { delete[] data; }
};

struct sslstream_data {
    ssldatachunk *inbox;                // pointer to the first unused data chunk
    uint8_t buffer[BUFFER_SIZE];

    sslstream_data() : inbox(NULL) { }
};

SSLBuffer::SSLBuffer(SSLSocket* socket) {
    this->socket = socket;
    m_data = new sslstream_data();
    m_data->inbox = new ssldatachunk(1);
    char_type *data = (char_type *)m_data->inbox->data;
    setg(data, data, data);
}

SSLBuffer::~SSLBuffer() {
    // free up any buffered data
    ssldatachunk *curchunk = m_data->inbox;
    while (curchunk != NULL) {
        ssldatachunk *old = curchunk;
        curchunk = curchunk->next;
        delete old;
    }
    delete m_data;
    m_data = NULL;
}

int SSLBuffer::underflow() {
    // check for more data; if we're a blocking socket and we don't have another
    // buffer waiting then block until more data arrives
    if (m_data->inbox->next == NULL) {
        // read the available data...
        int num = socket->read(m_data->buffer, BUFFER_SIZE);
        if (num == 0) {
            socket->close();
        }
        else {
            // ...and append it to the inbox
            ssldatachunk *curchunk = m_data->inbox, *newchunk = new ssldatachunk(num);
            memcpy(newchunk->data, m_data->buffer, num);
            if (curchunk != NULL) {
                while (curchunk->next != NULL)
                    curchunk = curchunk->next;
                curchunk->next = newchunk;
            } else {
                m_data->inbox = newchunk;
            }
        }
    }

    // bail if we don't have another buffer to swap to yet
    if (m_data->inbox == NULL || m_data->inbox->next == NULL) return EOF;

    // we've used up the buffer at the front of the inbox, so free it
    ssldatachunk *old = m_data->inbox;
    m_data->inbox = old->next;
    delete old;

    // and set the new buffer up for the stream to read from
    char_type *newdata = (char_type *)m_data->inbox->data;
    setg(newdata, newdata, newdata + m_data->inbox->bytes);
    return *newdata;
}

int SSLBuffer::overflow(int c) {
    return socket->write(&c, sizeof(c));
}

std::streamsize SSLBuffer::xsputn(const char *s, std::streamsize n) {
    return socket->write(s, n);
}

std::streamsize SSLBuffer::xsgetn(char *s, std::streamsize n) {
    return socket->read(s, n);
}
