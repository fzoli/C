/* 
 * File:   SSLStream.h
 * Author: zoli
 *
 * Created on 2013. szeptember 4., 18:55
 */

#ifndef SSLSTREAM_H
#define	SSLSTREAM_H

#include <streambuf>
#include <libio.h>

#include "Socket.h"

struct sb_data;

class SocketBuffer : public std::streambuf {
    
    public:
        
        explicit SocketBuffer(Socket* socket);
        virtual ~SocketBuffer();
        
    protected:
        
        virtual int underflow();
        virtual int overflow(int c = EOF);
        virtual std::streamsize xsputn(const char *s, std::streamsize n);
        virtual std::streamsize xsgetn(char *s, std::streamsize n);
        
    private:
        
        Socket* socket;
        sb_data * m_data;
        bool fillbuffer();
        
};

#endif	/* SSLSTREAM_H */
