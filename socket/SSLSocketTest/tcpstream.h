#ifndef _tcpstream_h
#define _tcpstream_h

// Copyright (C) Tane Pendragon 2010
// Released under the Creative Commons Attribution License (cc by)

// tcpstream - an iostream for socket communication
//
// example with blocking socket:
//
//   std::string line;
//   tcpstream sock;
//   sock.connect("www.google.com", "80");
//   if (sock.bad()) { /* barf */ }
//   sock << "GET / HTTP/1.0\r\nConnection: close\r\n\r\n";
//   while (sock.connected() || sock.hasline()) {
//     std::getline(sock, line);
//     std::cout << line << std::endl;
//   }
//
//
// example with non-blocking socket:
//
//   std::string line;
//   tcpstream sock;
//   sock.blocking(false);
//   sock.connect("www.google.com", "80");
//   if (sock.bad()) handleError(sock);
//   sock << "GET / HTTP/1.0\r\nConnection: close\r\n\r\n";
//   while (sock.connected()) {
//     if (sock.hasline()) {
//       std::getline(sock, line);
//       cout << line;
//     }
//     cout << "No line yet (" << sock.numbytes() << "bytes buffered)." << endl;
//     sleep(1);
//   }
//
//
// server example:
//
//   std::string line;
//   tcplistener listener;
//   if (!listener.listen("9999")) handleError();
//   do {
//     if (listener.pending()) {
//       tcpstream sock;
//       listener.accept(sock);
//       cout << "Incoming connection!" << endl;
//       if (sock.connected()) {
//         cout << "Waiting for 'Polo!'" << endl;
//         sock << "Marco?" << endl;
//         std::getline(sock, line);
//         cout << "Remote computer said: " << line << endl;
//       }
//       sock.close();
//     } else {
//       sleep(1);
//   } while (line != "Polo!");
//   cout << "Got it!" << endl;
//   listener.close();
//

#include <streambuf>
#include <iostream>
#include <cstdio>
#include <ios>

// forward declarations
struct tcpbuf_data;
struct tcplistener_data;

// tcpbuf - a std::streambuf for a TCP stream
class tcpbuf : public std::streambuf {
public:
    explicit tcpbuf();
    virtual ~tcpbuf();

    // set the buffer to block, or not, on socket input
    void blocking(bool block);

    // connect to the given host and port number
    bool connect(const std::string& host, const std::string& port);

    // returns true if this socket is connected
    bool connected();

    // returns true if there is a complete line in the read buffer
    bool hasline();

    // returns the number of bytes of data waiting to be read
    int hasdata();

    // get the last error to occur
    const std::string& error();

    // get the name of the remote host
    const std::string& remotehost();

    // closes the connection and resets the stream
    void close();

protected:
    virtual int underflow();
    virtual int overflow(int c = EOF);
    virtual std::streamsize xsputn(const char *s, std::streamsize n);
    virtual std::streamsize xsgetn(char *s, std::streamsize n);

    // tops up the buffer with any data waiting in the socket
    bool fillbuffer(bool block = false);

    // prevent assignment, since that would be nonsensical
    const tcpbuf& operator=(const tcpbuf& other);

protected:
    tcpbuf_data * m_data;

    friend class tcplistener;
};

// tcpstream - a tcpbuf-based iostream, for convenience
class tcpstream : public std::iostream, public tcpbuf {
public:
    tcpstream() : std::iostream(this) { }
};

// tcplistener - a class which listens on a port and accepts connections
class tcplistener {
public:
    tcplistener(int backlog = 3);
    ~tcplistener();

    // listen on the given port for incoming socket connections
    bool listen(const std::string& port);

    // returns true if the listener is listening
    bool listening();

    // returns true if any incoming connections are pending
    bool pending();

    // accept an incoming socket connection on the supplied socket
    // if no connection is pending, this will block until a connection is made
    bool accept(tcpstream& sock);

    // stop listening for connections
    void close();

private:
    tcplistener_data * m_data;
};

#endif // _tcpstream_h
