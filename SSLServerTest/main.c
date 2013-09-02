/**
 * Server side
 * Based on:
 * http://savetheions.com/2010/01/16/quickly-using-openssl-in-c/
 * http://stackoverflow.com/questions/7698488/turn-a-simple-socket-into-an-ssl-socket
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

#include <pthread.h>

typedef struct {
    int socket;
    SSL *sslHandle;
} connection;

typedef struct {
    int socket;
    struct sockaddr_in server;
    int serverlen;
} handles;

#define PORT 9443
#define MAXCONNECTIONS 5

// Certificate file paths
#define CA_PATH   "/home/zoli/test-certs/ca.crt"
#define CRT_PATH  "/home/zoli/test-certs/bridge.crt"
#define KEY_PATH  "/home/zoli/test-certs/bridge.key"
#define KEY_PASS  "asdfgh"

// Establish a regular tcp connection with a client
handles *tcpServerOpen() {
    int handle;

    handles *h;
    h = malloc(sizeof (handles));
    h->socket = 0;
    
    handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle == -1) {
        perror("Socket");
    }
    else {
        
        // Sets REUSEADDR option to avoid socket bind after the application closes.
        const int       optVal = 1;
        const socklen_t optLen = sizeof(optVal);
        setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
        
        h->server.sin_family = AF_INET;
        h->server.sin_addr.s_addr = INADDR_ANY;
        h->server.sin_port = htons(PORT);
        int server_length = sizeof (h->server);
        if (bind(handle, (struct sockaddr *) &h->server, server_length) == -1) {
            perror("Bind");
            return h;
        }
        if (listen(handle, MAXCONNECTIONS) == -1) {
            perror("Listen");
            return h;
        }
        h->socket = handle;
        h->serverlen = server_length;
        printf("Server is listening\n");
    }

    return h;
}

int tcpAccept(handles *h) {
    if (h->socket == 0) return 0;
    int newhandle = accept(h->socket, (struct sockaddr *) &h->server, (socklen_t *) &h->serverlen);
    if (newhandle <= 0) {
        perror("Accept");
        return 0;
    }
    return newhandle;
}

// Establish a connection using an SSL layer
connection *sslAccept(handles *h, SSL_CTX* ctx) {
    connection *c;
    c = malloc(sizeof (connection));
    c->sslHandle = NULL;
    
    if (ctx != NULL) {
        c->socket = tcpAccept(h);
        if (c->socket) {
            // Create an SSL struct for the connection
            c->sslHandle = SSL_new(ctx);
            if (c->sslHandle == NULL)
                ERR_print_errors_fp(stderr);

            // Connect the SSL struct to our connection
            if (!SSL_set_fd(c->sslHandle, c->socket))
                ERR_print_errors_fp(stderr);

            // Initiate SSL handshake
            if (SSL_accept(c->sslHandle) != 1)
                ERR_print_errors_fp(stderr);
        }
        else {
            perror("Server connect failed");
        }
    }
    return c;
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

void tcpServerClose(handles *h) {
    if (h->socket) close(h->socket);
    free(h);
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

void testHandle(connection *c) {
    sslWrite(c, "GET /\r\n\r\n");
    
    sleep(5);
    
    char *response = sslRead(c);
    if (response) printf("\n%s\n", response);
    free(response);
    
    sslDisconnect(c);
}

// Very basic main: we send GET / and print the response.
int main(int argc, char **argv) {
    sslInit();
    
    SSL_CTX* ctx = sslCreateCtx(SSLv23_server_method());
    if (ctx == NULL) return EXIT_FAILURE;

    handles *h = tcpServerOpen();
    
    connection *c;
    int i = 0;
    
    while (h->socket && i < 5) {
        c = sslAccept(h, ctx);
        
        pthread_t testHandleThread;
        if (pthread_create(&testHandleThread, NULL, (void *) &testHandle, c)) {
            fprintf(stderr, "Error creating thread\n");
        }
        else if (!(i+1 < 5)) {
            if (pthread_join(testHandleThread, NULL)) {
                fprintf(stderr, "Error joining thread\n");
            }
        }
        
        i++;
    }
    
    tcpServerClose(h);
    
    sslDestroyCtx(ctx);
    
    sslDestroy();
    
    return EXIT_SUCCESS;
}
