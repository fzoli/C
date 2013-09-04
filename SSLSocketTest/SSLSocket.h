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

#include <stdint.h>

#include <string>

//#include "SSLStream.h"

class SSLSocket {
    
    protected:
        
        struct  connection {
            int socket;
            SSL *sslHandle;
        };
        
    public:
        
        SSLSocket(connection c);
        SSLSocket(const char *host, uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        virtual ~SSLSocket();
        
//        SSLStream* getStream();
        bool isClosed();
        virtual void close();
        void write(int byte);
        int write(const char *text) const;
        int write(const void *buf, int num) const;
        int read();
        int read(void *buf, int num) const;
        void read (std::string& s) const;
//        void setNonBlocking(bool b);
        
        const SSLSocket& operator >> (std::string& s) const;
        const SSLSocket& operator << (const std::string& s) const;
        
    private:
        
        connection conn;
//        SSLStream* stream;
        static int count;
        static pthread_mutex_t mutexCount;
        
        static void loadSSL();
        static void unloadSSL();
        static int tcpConnect(const char *addr, uint16_t port);
        void sslConnect(const char *addr, uint16_t port);
        static void sslDisconnect(connection c);
//        static void setNonBlocking(int sock, bool b);
        
    protected:
        
        bool closed;
        SSL_CTX* ctx;
        
        SSLSocket();
        
        static SSL_CTX *sslCreateCtx(bool client, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd);
        static void sslDestroyCtx(SSL_CTX *sctx);
        
};

#endif	/* SSLSOCKET_H */
