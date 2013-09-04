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
        
        SSLServerSocket(uint16_t port, int maxConn, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        
        SSLSocket accept();
        void close();
        
    private:
        
        handles h;
        
        void open(uint16_t port, int maxConn);
        int tcpAccept();
        connection sslAccept();
        
};

#endif	/* SSLSERVERSOCKET_H */
