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

typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;

#define PORT 9443
#define MAXCONNECTIONS 5

// Certificate file paths
#define CA_PATH   "/home/zoli/test-certs/ca.crt"
#define CRT_PATH  "/home/zoli/test-certs/bridge.crt"
#define KEY_PATH  "/home/zoli/test-certs/bridge.key"
#define KEY_PASS  "asdfgh"

// Establish a regular tcp connection with a client
int tcpConnect ()
{
  int handle, newhandle;
  struct sockaddr_in server;

  handle = socket (AF_INET, SOCK_STREAM, 0);
  if (handle == -1)
    {
      perror ("Socket");
      handle = 0;
    }
  else
    {
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port = htons (PORT);
      int server_length = sizeof ( server );
      if (bind(handle, ( struct sockaddr * ) &server, server_length) == -1) {
          perror ("Bind");
          return 0;
      }
      if (listen(handle, MAXCONNECTIONS) == -1) {
          perror ("Listen");
          return 0;
      }
      newhandle = accept(handle, ( struct sockaddr * ) &server, ( socklen_t * ) &server_length);
      if ( newhandle <= 0 )
        {
          perror ("Accept");
          return 0;
        }
    }

  return newhandle;
}

// Establish a connection using an SSL layer
connection *sslConnect (void)
{
  connection *c;

  c = malloc (sizeof (connection));
  c->sslHandle = NULL;
  c->sslContext = NULL;

  c->socket = tcpConnect ();
  if (c->socket)
    {
      // Register the error strings for libcrypto & libssl
      SSL_load_error_strings ();
      // Register the available ciphers and digests
      SSL_library_init ();

      // New context saying we are a client, and using SSL 2 or 3
      c->sslContext = SSL_CTX_new (SSLv23_server_method ());
      if (c->sslContext == NULL)
        ERR_print_errors_fp (stderr);
      
      // Generate a new DH key during each handshake
      SSL_CTX_set_options(c->sslContext, SSL_OP_SINGLE_DH_USE);
      
      // The verification of certificates is activated
      SSL_CTX_set_verify(c->sslContext, SSL_VERIFY_PEER, NULL);
      
      // Sets the password of the private key
      SSL_CTX_set_default_passwd_cb_userdata(c->sslContext, KEY_PASS);
      
      if (SSL_CTX_load_verify_locations(c->sslContext, CA_PATH, NULL) == 0)
          perror ("Ca");
      
      if (SSL_CTX_use_certificate_file(c->sslContext, CRT_PATH, SSL_FILETYPE_PEM) == 0)
          perror ("Crt");
      
      if (SSL_CTX_use_PrivateKey_file(c->sslContext, KEY_PATH, SSL_FILETYPE_PEM) == 0)
          perror ("Key");
      
      // Create an SSL struct for the connection
      c->sslHandle = SSL_new (c->sslContext);
      if (c->sslHandle == NULL)
        ERR_print_errors_fp (stderr);

      // Connect the SSL struct to our connection
      if (!SSL_set_fd (c->sslHandle, c->socket))
        ERR_print_errors_fp (stderr);

      // Initiate SSL handshake
      if (SSL_accept (c->sslHandle) != 1)
        ERR_print_errors_fp (stderr);
    }
  else
    {
      perror ("Server connect failed");
    }

  return c;
}

// Disconnect & free connection struct
void sslDisconnect (connection *c)
{
  if (c->socket)
    close (c->socket);
  if (c->sslHandle)
    {
      SSL_shutdown (c->sslHandle);
      SSL_free (c->sslHandle);
    }
  if (c->sslContext)
    SSL_CTX_free (c->sslContext);

  free (c);
}

// Read all available text from the connection
char *sslRead (connection *c)
{
  const int readSize = 1024;
  char *rc = NULL;
  int received, count = 0;
  char buffer[1024];

  if (c)
    {
      while (1)
        {
          if (!rc)
            rc = malloc (readSize * sizeof (char) + 1);
          else
            rc = realloc (rc, (count + 1) *
                          readSize * sizeof (char) + 1);

          received = SSL_read (c->sslHandle, buffer, readSize);
          buffer[received] = '\0';

          if (received > 0)
            strcat (rc, buffer);

          if (received < readSize)
            break;
          count++;
        }
    }

  return rc;
}

// Write text to the connection
void sslWrite (connection *c, char *text)
{
  if (c)
    SSL_write (c->sslHandle, text, strlen (text));
}

// Very basic main: we send GET / and print the response.
int main (int argc, char **argv)
{
  connection *c;
  char *response;

  c = sslConnect ();
  
  sslWrite (c, "GET /\r\n\r\n");
  
  response = sslRead (c);

  printf ("%s\n", response);

  sslDisconnect (c);
  free (response);

  return EXIT_SUCCESS;
}
