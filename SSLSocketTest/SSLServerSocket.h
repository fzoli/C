/* 
 * File:   SSLServerSocket.h
 * Author: zoli
 *
 * Created on 2013. szeptember 2., 20:29
 */

#ifndef SSLSERVERSOCKET_H
#define	SSLSERVERSOCKET_H

#include "SSLSocket.h"

#include <netinet/in.h>

class SSLServerSocket : private SSLSocket {
    
    struct handles {
        int socket;
        struct sockaddr_in server;
        int serverlen;
    };
    
    public:
        
        SSLServerSocket(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        SSLServerSocket(uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile);
        
        SSLSocket accept();
        void close();
        
    private:
        
        handles h;
        
        void init(uint16_t port, uint16_t maxNewConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        void open(uint16_t port, uint16_t maxConn);
        int tcpAccept();
        connection sslAccept();
        
};

#endif	/* SSLSERVERSOCKET_H */
