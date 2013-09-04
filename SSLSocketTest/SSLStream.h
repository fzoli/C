/* 
 * File:   SSLStream.h
 * Author: zoli
 *
 * Created on 2013. szeptember 4., 18:55
 */

#ifndef SSLSTREAM_H
#define	SSLSTREAM_H

#include <streambuf>

#include "SSLSocket.h"

struct sslstream_data;

class SSLStream : public std::streambuf {
    
    public:
        
        explicit SSLStream(SSLSocket* socket);
        virtual ~SSLStream();
        
    protected:
        
        virtual int underflow();
        virtual int overflow(int c = -1);
        virtual std::streamsize xsputn(const char *s, std::streamsize n);
        virtual std::streamsize xsgetn(char *s, std::streamsize n);
        
    private:
        
        SSLSocket* socket;
        sslstream_data * m_data;
        
};

#endif	/* SSLSTREAM_H */
