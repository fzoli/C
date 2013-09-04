/**
 * Client side
 * Based on http://savetheions.com/2010/01/16/quickly-using-openssl-in-c/
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <stdlib.h>

// Simple structure to keep track of the handle, and
// of what needs to be freed later.
typedef struct {
    int socket;
    SSL *sslHandle;
} connection;

// For this example, we'll be testing on openssl.org
#define SERVER  "localhost"
#define PORT 9443

// Certificate file paths
#define CA_PATH   "/home/zoli/test-certs/ca.crt"
#define CRT_PATH  "/home/zoli/test-certs/controller.crt"
#define KEY_PATH  "/home/zoli/test-certs/controller.key"
#define KEY_PASS  "asdfgh"

// Establish a regular tcp connection
int tcpConnect() {
    int error, handle;
    struct hostent *host;
    struct sockaddr_in server;

    host = gethostbyname(SERVER);
    handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle == -1) {
        perror("Socket");
        handle = 0;
    }
    else {
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr = *((struct in_addr *) host->h_addr);
        bzero(&(server.sin_zero), 8);

        error = connect(handle, (struct sockaddr *) &server,
                sizeof (struct sockaddr));
        if (error == -1) {
            perror("Connect");
            handle = 0;
        }
    }

    return handle;
}

// Establish a connection using an SSL layer
connection *sslConnect(SSL_CTX* ctx) {
    if (ctx == NULL) return NULL;
    
    connection *c;
    c = malloc(sizeof (connection));
    c->sslHandle = NULL;

    c->socket = tcpConnect();

    if (c->socket) {
        // Create an SSL struct for the connection
        c->sslHandle = SSL_new(ctx);
        if (c->sslHandle == NULL)
            ERR_print_errors_fp(stderr);
        // Connect the SSL struct to our connection
        if (!SSL_set_fd(c->sslHandle, c->socket))
            ERR_print_errors_fp(stderr);

        // Initiate SSL handshake
        if (SSL_connect(c->sslHandle) != 1)
            ERR_print_errors_fp(stderr);
    }
    else {
        perror("Connect failed");
    }

    return c;
}

void sslInit() {
    // Register the error strings for libcrypto & libssl
    SSL_load_error_strings();
    // Register the available ciphers and digests
    SSL_library_init();
}

void sslDestroy() {
    ERR_free_strings();
    EVP_cleanup();
}

// Disconnect & free connection struct
void sslDisconnect(connection *c) {
    if (c->socket)
        close(c->socket);
    if (c->sslHandle) {
        SSL_shutdown(c->sslHandle);
        SSL_free(c->sslHandle);
    }
    free(c);
}

SSL_CTX *sslCreateCtx(SSL_METHOD *method) {
    // New context saying we are the server, and using SSL 2 or 3
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
    }
    else {
        // Generate a new DH key during each handshake
        SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);

        // The verification of certificates is activated
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

        // Sets the password of the private key
        SSL_CTX_set_default_passwd_cb_userdata(ctx, KEY_PASS);

        if (SSL_CTX_load_verify_locations(ctx, CA_PATH, NULL) == 0) {
            perror("Ca");
            ctx = NULL;
        }

        if (SSL_CTX_use_certificate_file(ctx, CRT_PATH, SSL_FILETYPE_PEM) == 0) {
            perror("Crt");
            ctx = NULL;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx, KEY_PATH, SSL_FILETYPE_PEM) == 0) {
            perror("Key");
            ctx = NULL;
        }
    }
    return ctx;
}

void sslDestroyCtx(SSL_CTX *ctx) {
    if (ctx) SSL_CTX_free(ctx);
}

// Read all available text from the connection
char *sslRead(connection *c) {
    const int readSize = 1024;
    char *rc = NULL;
    int received, count = 0;
    char buffer[1024];

    if (c->socket) {
        while (1) {
            if (!rc)
                rc = malloc(readSize * sizeof (char) + 1);
            else
                rc = realloc(rc, (count + 1) *
                    readSize * sizeof (char) + 1);

            received = SSL_read(c->sslHandle, buffer, readSize);
            buffer[received] = '\0';

            if (received > 0)
                strcat(rc, buffer);

            if (received < readSize)
                break;
            count++;
        }
    }

    return rc;
}

// Write text to the connection
void sslWrite(connection *c, char *text) {
    if (c->socket) SSL_write(c->sslHandle, text, strlen(text));
}

void sslWriteByte(connection *c, int byte) {
    if (byte < 0 || byte > 255) return;
    char unsigned byte_char = byte;
    if (c->socket) SSL_write(c->sslHandle, &byte_char, 1);
}

// Very basic main: we send GET / and print the response.
int main(int argc, char **argv) {
    sslInit();

    SSL_CTX* ctx = sslCreateCtx(SSLv23_client_method());
    if (ctx == NULL) return EXIT_FAILURE;
    
    connection *c;
    char *response;

    c = sslConnect(ctx);
    
    sslWriteByte(c, 1);
    sslWriteByte(c, 0);
    
    sslWrite(c, "OK\r\n");
    
    response = sslRead(c);

    if (response) printf("%s\n", response);

    sleep(2);
    
    free(response);

    sslDisconnect(c);
    
    sslDestroyCtx(ctx);
    
    sslDestroy();
    
    return EXIT_SUCCESS;
}
