/* 
 * File:   SSLSocket.h
 * Author: zoli
 *
 * Created on 2013. szeptember 2., 19:58
 */

#ifndef SSLSOCKET_H
#define	SSLSOCKET_H

#include <openssl/ssl.h>

#include <pthread.h>

#include "Socket.h"

class SSLSocket : public Socket {
    
    protected:
        
        struct  connection {
            int socket;
            SSL *sslHandle;
        };
        
    public:
        
        SSLSocket(connection c);
        SSLSocket(const char *host, uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, int timeout = 0, bool verify = true);
        virtual ~SSLSocket();
        
        char* getClientName();
        char* getServerName();
        void close();
        int write(const void *buf, int num) const;
        int read(void *buf, int num) const;
        
        using Socket::write;
        using Socket::read;
        
    private:
        
        connection conn;
        char *clientName, *serverName;
        static int count;
        static pthread_mutex_t mutexCount;
        
        static void loadSSL();
        static void unloadSSL();
        static char *getCommonName(X509 *cert);
        int sslConnect(const char *addr, uint16_t port, int timeout);
        static void sslDisconnect(connection c);
        
    protected:
        
        SSL_CTX* ctx;
        
        SSLSocket();
        
        static SSL_CTX *sslCreateCtx(bool client, bool verify, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        static void sslDestroyCtx(SSL_CTX *sctx);
        
};

#endif	/* SSLSOCKET_H */
