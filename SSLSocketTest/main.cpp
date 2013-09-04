/* 
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2013. szeptember 1., 19:59
 */

#include <iostream>

#include "SSLServerSocket.h"
#include "CertificateException.h"

#include "tcpstream.h"

#define PORT 9443

using namespace std;

int waitServer = 1;

string path(string filename) {
    char buff[1024];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1) {
        buff[len] = '\0';
        string s(buff);
        return s.substr(0, s.find_last_of("/") + 1).append(filename);
    }
    throw "path error";
}

void* server(void*) {
    string CAfile = path("ca.crt");
    string CRTfile = path("server.crt");
    string KEYfile = path("server.key");
    char *KEYpass = (char *) "asdfgh";
    try {
        SSLServerSocket s(PORT, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass);
        waitServer = 0;
        SSLSocket c = s.accept();
        c << "Hello World!";
        c.close();
        s.close();
    }
    catch (CertificateException ex) {
        cerr << "Certificate exception: " + ex.msg() + "\n";
    }
    catch (SSLSocketException ex) {
        cerr << "SSL Server Socket exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SocketException ex) {
        cerr << "Socket Server exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    return NULL;
}

void* client(void*) {
    string CAfile = path("ca.crt");
    string CRTfile = path("client.crt");
    string KEYfile = path("client.key");
    char *KEYpass = (char *) "asdfgh";
    try {
        SSLSocket c("localhost", PORT, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass);
        string reply;
        c >> reply;
        cout << reply + "\n";
        c.close();
    }
    catch (CertificateException ex) {
        cerr << "Certificate exception: " + ex.msg() + "\n";
    }
    catch (SSLSocketException ex) {
        cerr << "SSL Socket exception: " + ex.msg() + "\n";
    }
    catch (SocketException ex) {
        cerr << "Socket exception: " + ex.msg() + "\n";
    }
    return NULL;
}

void streamTest() {
    std::string line;
    tcpstream sock;
    sock.connect("localhost", "80");
    if (sock.bad()) {
        cerr << "Localhost is not available.\n";
        return;
    }
    sock << "GET /\r\n\r\n";
    while (sock.connected()) {
       std::getline(sock, line);
       cout << line;
    }
    cout << "\n";
}

/*
 * 
 */
int main(int argc, char** argv) {
    streamTest();
    pthread_t serverThread;
    if (pthread_create(&serverThread, NULL, &server, NULL)) {
        cerr << "Server thread could not be created.";
        return EXIT_FAILURE;
    }
    while(waitServer == 1) {
        usleep(1);
    }
    if (waitServer == -1) {
        return EXIT_FAILURE;
    }
    pthread_t clientThread;
    if (pthread_create(&clientThread, NULL, &client, NULL)) {
        cerr << "Client thread could not be created.\n";
        return EXIT_FAILURE;
    }
    if (pthread_join(serverThread, NULL)) {
        cerr << "Could not join server thread.\n";
        return EXIT_FAILURE;
    }
    if (pthread_join(clientThread, NULL)) {
        cerr << "Could not join client thread.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
