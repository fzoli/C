/* 
 * File:   SSLServerSocket.h
 * Author: zoli
 *
 * Created on 2013. szeptember 2., 20:29
 */

#ifndef SSLSERVERSOCKET_H
#define	SSLSERVERSOCKET_H

#include "SSLSocket.h"
#include "ServerSocket.h"

class SSLServerSocket : private SSLSocket, private ServerSocket {
    
    public:
        
        SSLServerSocket(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify = true);
        SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify = true);
        SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, bool verify = true);
        
        SSLSocket accept();
        void close();
        bool isClosed();
        void setTimeout(int sec);
        
    private:
        
        void init(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, bool verify = true);
        connection sslAccept();
        
};

#endif	/* SSLSERVERSOCKET_H */
