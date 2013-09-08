/* 
 * File:   SSLSocket.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 2., 19:58
 */

#include "SSLSocket.h"
#include "SocketBuffer.h"
#include "CertificateException.h"

#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/err.h>
#include <iostream>

int SSLSocket::count = 0;
pthread_mutex_t SSLSocket::mutexCount = PTHREAD_MUTEX_INITIALIZER;

SSLSocket::SSLSocket() {
    loadSSL();
    clientName = NULL;
    serverName = NULL;
}

SSLSocket::~SSLSocket() {
    if (clientName != NULL) delete [] clientName;
    if (serverName != NULL) delete [] serverName;
    unloadSSL(); // TODO: segfault több szál esetén
}

SSLSocket::SSLSocket(connection c) {
    loadSSL();
    ctx = NULL;
    m_sock = c.socket;
    conn.socket = c.socket;
    conn.sslHandle = c.sslHandle;
    buffer = new SocketBuffer(this);
    clientName = getCommonName(SSL_get_peer_certificate(conn.sslHandle));
    serverName = getCommonName(SSL_get_certificate(conn.sslHandle));
}

SSLSocket::SSLSocket(const char *host, uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd, int timeout, bool verify) {
    loadSSL();
    ctx = sslCreateCtx(true, verify, CAfile, CRTfile, KEYfile, passwd);
    m_sock = sslConnect(host, port, timeout);
    buffer = new SocketBuffer(this);
    clientName = getCommonName(SSL_get_certificate(conn.sslHandle));
    serverName = getCommonName(SSL_get_peer_certificate(conn.sslHandle));
}

char* SSLSocket::getClientName() {
    return clientName;
}

char* SSLSocket::getServerName() {
    return serverName;
}

void SSLSocket::loadSSL() {
    pthread_mutex_lock(&mutexCount);
    if (count++ <= 0) {
        SSL_load_error_strings();
        SSL_library_init();
    }
    pthread_mutex_unlock(&mutexCount);
}

void SSLSocket::unloadSSL() {
    pthread_mutex_lock(&mutexCount);
    if (--count == 0) {
        ERR_free_strings();
        EVP_cleanup();
        ERR_free_strings();
        EVP_cleanup();
    }
    pthread_mutex_unlock(&mutexCount);
}

SSL_CTX *SSLSocket::sslCreateCtx(bool client, bool verify, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd) {
    SSL_CTX *sctx = SSL_CTX_new(client ? SSLv23_client_method() : SSLv23_server_method());
    if (sctx == NULL) {
        throw SSLSocketException ( "Could not create SSL context." );
    }
    else {
        // Generate a new DH key during each handshake
        SSL_CTX_set_options(sctx, SSL_OP_SINGLE_DH_USE);

        // The verification contextof certificates is activated
        if (verify) SSL_CTX_set_verify(sctx, SSL_VERIFY_PEER, NULL);

        // Sets the password of the private key
        SSL_CTX_set_default_passwd_cb_userdata(sctx, passwd);

        if (!client) {
            int s_server_session_id_context = 1;
            SSL_CTX_set_session_id_context(sctx, (const unsigned char*) &s_server_session_id_context, sizeof(s_server_session_id_context));
        }
        
        if (SSL_CTX_load_verify_locations(sctx, CAfile, NULL) == 0) {
            throw CertificateException ( "CA file could not be loaded." );
        }

        if (SSL_CTX_use_certificate_file(sctx, CRTfile, SSL_FILETYPE_PEM) == 0) {
            ERR_print_errors_fp(stderr);
            throw CertificateException ( "CRT file could not be loaded." );
        }

        if (SSL_CTX_use_PrivateKey_file(sctx, KEYfile, SSL_FILETYPE_PEM) == 0) {
            throw CertificateException ( "KEY file could not be loaded." );
        }
    }
    
    return sctx;
}

void SSLSocket::sslDestroyCtx(SSL_CTX *sctx) {
    if (sctx != NULL && sctx) SSL_CTX_free(sctx);
}

void SSLSocket::sslDisconnect(connection c) {
    if (c.socket)
        ::close(c.socket);
    if (c.sslHandle != NULL && c.sslHandle) {
        SSL_shutdown(c.sslHandle);
        SSL_free(c.sslHandle);
    }
}

// Establish a connection using an SSL layer
int SSLSocket::sslConnect(const char *addr, uint16_t port, int timeout) {
    conn.sslHandle = NULL;
    conn.socket = tcpConnect(addr, port, timeout);
    
    // Create an SSL struct for the connection
    conn.sslHandle = SSL_new(ctx);
    if (conn.sslHandle == NULL) {
        close();
        throw SSLSocketException ( "Could not create SSL object." );
    }
    // Connect the SSL struct to our connection
    if (!SSL_set_fd(conn.sslHandle, conn.socket)) {
        close();
        throw SSLSocketException ( "Could not connect the SSL object to the socket." );
    }
    // Initiate SSL handshake
    if (SSL_connect(conn.sslHandle) != 1) {
        close();
        throw SSLSocketException ( "Error during SSL handshake." );
    }
    return conn.socket;
}

char *SSLSocket::getCommonName(X509 *cert) {
    if (cert == NULL) return NULL;
    X509_NAME *subjectName;
    char  *subjectCn = new char[256];
    subjectName = X509_get_subject_name(cert);
    if (X509_NAME_get_text_by_NID(subjectName, NID_commonName, subjectCn, 256) != -1) return subjectCn;
    delete [] subjectCn;
    throw CertificateException( "Could not get common name." );
}

void SSLSocket::close() {
    if (closed) return;
    closed = true;
    sslDisconnect(conn);
    sslDestroyCtx(ctx);
}

int SSLSocket::write(const void *buf, int num) const {
    int status = SSL_write(conn.sslHandle, buf, num);
    if (status <= 0) throw SSLSocketException( "Could not write byte" );
    return status;
}

int SSLSocket::read(void *buf, int num) const {
    int status = SSL_read(conn.sslHandle, buf, num);
    if (status < 0) throw SSLSocketException ( "Could not read from socket." );
    return status;
}
