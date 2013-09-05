/* 
 * File:   SSLSocket.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 2., 19:58
 */

#include "SSLSocket.h"
#include "SSLBuffer.h"
#include "CertificateException.h"

#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <openssl/err.h>
#include <iostream>

int SSLSocket::count = 0;
pthread_mutex_t SSLSocket::mutexCount = PTHREAD_MUTEX_INITIALIZER;

SSLSocket::SSLSocket() : closed(false) {
    loadSSL();
    buffer = NULL;
    clientName = NULL;
    serverName = NULL;
}

SSLSocket::~SSLSocket() {
    unloadSSL();
}

SSLSocket::SSLSocket(connection c) : closed(false) {
    loadSSL();
    ctx = NULL;
    conn.socket = c.socket;
    conn.sslHandle = c.sslHandle;
    buffer = new SSLBuffer(this);
    clientName = getCommonName(SSL_get_peer_certificate(conn.sslHandle));
    serverName = getCommonName(SSL_get_certificate(conn.sslHandle));
}

SSLSocket::SSLSocket(const char *host, uint16_t port, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd) : closed(false) {
    loadSSL();
    ctx = sslCreateCtx(true, CAfile, CRTfile, KEYfile, passwd);
    sslConnect(host, port);
    buffer = new SSLBuffer(this);
    clientName = getCommonName(SSL_get_certificate(conn.sslHandle));
    serverName = getCommonName(SSL_get_peer_certificate(conn.sslHandle));
}

std::streambuf* SSLSocket::getBuffer() {
    return buffer;
}

char* SSLSocket::getClientName() {
    return clientName;
}

char* SSLSocket::getServerName() {
    return serverName;
}

bool SSLSocket::isClosed() {
    return closed;
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

SSL_CTX *SSLSocket::sslCreateCtx(bool client, const char *CAfile, const char *CRTfile, const char *KEYfile, void *passwd) {
    SSL_CTX *sctx = SSL_CTX_new(client ? SSLv23_client_method() : SSLv23_server_method());
    if (sctx == NULL) {
        throw SSLSocketException ( "Could not create SSL context." );
    }
    else {
        // Generate a new DH key during each handshake
        SSL_CTX_set_options(sctx, SSL_OP_SINGLE_DH_USE);

        // The verification contextof certificates is activated
        SSL_CTX_set_verify(sctx, SSL_VERIFY_PEER, NULL);

        // Sets the password of the private key
        SSL_CTX_set_default_passwd_cb_userdata(sctx, passwd);

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
    if (sctx) SSL_CTX_free(sctx);
}

void SSLSocket::sslDisconnect(connection c) {
    if (c.socket)
        ::close(c.socket);
    if (c.sslHandle) {
        SSL_shutdown(c.sslHandle);
        SSL_free(c.sslHandle);
    }
}

int SSLSocket::tcpConnect(const char *addr, uint16_t port) {
    struct hostent *host = gethostbyname(addr);
    int handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle == -1) {
        throw SocketException ( "Could not create socket." );
    }
    else {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr = *((struct in_addr *) host->h_addr);
        bzero(&(server.sin_zero), 8);
        if (connect(handle, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1) {
            throw SocketException ( "Could not connect to the server." );
        }
    }
    return handle;
}

// Establish a connection using an SSL layer
void SSLSocket::sslConnect(const char *addr, uint16_t port) {
    conn.sslHandle = NULL;
    conn.socket = tcpConnect(addr, port);
    
    // Create an SSL struct for the connection
    conn.sslHandle = SSL_new(ctx);
    if (conn.sslHandle == NULL)
        throw SSLSocketException ( "Could not create SSL object." );
    
    // Connect the SSL struct to our connection
    if (!SSL_set_fd(conn.sslHandle, conn.socket))
        throw SSLSocketException ( "Could not connect the SSL object to the socket." );

    // Initiate SSL handshake
    if (SSL_connect(conn.sslHandle) != 1)
        throw SSLSocketException ( "Error during SSL handshake." );
}

char *SSLSocket::getCommonName(X509 *cert) {
    if (cert == NULL) throw CertificateException( "Certificate is NULL." );
    X509_NAME *subjectName;
    char  *subjectCn = new char[256];
    subjectName = X509_get_subject_name(cert);
    if (X509_NAME_get_text_by_NID(subjectName, NID_commonName, subjectCn, 256) != -1) return subjectCn;
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
    if (status <= 0)
        throw SSLSocketException( "Could not write byte" );
    return status;
}

int SSLSocket::write(const char *text) const {
    return write(text, strlen(text));
}

void SSLSocket::write(int byte) {
    if (byte < 0 || byte > 255) throw SocketException( "Byte out of range" );
    char unsigned byte_char = byte;
    write(&byte_char, 1);
}

int SSLSocket::read() {
    char buf[1];
    int status = read(&buf, 1);
    return status == 0 ? -1 : buf[0];
}

int SSLSocket::read(void *buf, int num) const {
    int status = SSL_read(conn.sslHandle, buf, num);
    if (status < 0) throw SocketException ( "Could not read from socket." );
    return status;
}

void SSLSocket::read(std::string& s) const {
    int status, bufsize = 1000;
    char buf[bufsize + 1];
    memset(buf, 0, bufsize + 1);
    s = "";
    
    std::ostringstream ss;
    do {
        status = read(buf, bufsize);
        ss.write(buf, status);
    } while (status > 0);
    
    s = ss.str().c_str();
    ss.clear();
}

const SSLSocket& SSLSocket::operator >> (std::string& s) const {
    read(s);
    return *this;
}

const SSLSocket& SSLSocket::operator << (const std::string& s) const {
    write(s.c_str());
    return *this;
}
