/* 
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2013. szeptember 1., 19:59
 */

#include <cstdlib>
#include <iostream>

#include "SSLServerSocket.h"
#include "CertificateException.h"

#define PORT 9443
#define MAXCONNECTIONS 5

using namespace std;

int waitServer = 1;

std::string path(std::string filename) {
    char buff[1024];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1) {
        buff[len] = '\0';
        std::string s(buff);
        return s.substr(0, s.find_last_of("/") + 1).append(filename);
    }
    throw "path error";
}

void* server(void*) {
    std::string CAfile = path("ca.crt");
    std::string CRTfile = path("server.crt");
    std::string KEYfile = path("server.key");
    char *KEYpass = (char *) "asdfgh";
    try {
        SSLServerSocket s(PORT, MAXCONNECTIONS, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass);
        waitServer = 0;
        SSLSocket c = s.accept();
        c << "Hello World!";
        c.close();
        s.close();
    }
    catch (CertificateException ex) {
        cout << "Certificate exception: " + ex.msg() + "\n";
    }
    catch (SSLSocketException ex) {
        cout << "SSL Server Socket exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SocketException ex) {
        cout << "Socket Server exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    return NULL;
}

void* client(void*) {
    std::string CAfile = path("ca.crt");
    std::string CRTfile = path("client.crt");
    std::string KEYfile = path("client.key");
    char *KEYpass = (char *) "asdfgh";
    try {
        SSLSocket c("localhost", PORT, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass);
        std::string reply;
        c >> reply;
        cout << reply + "\n";
        c.close();
    }
    catch (CertificateException ex) {
        cout << "Certificate exception: " + ex.msg() + "\n";
    }
    catch (SSLSocketException ex) {
        cout << "SSL Socket exception: " + ex.msg() + "\n";
    }
    catch (SocketException ex) {
        cout << "Socket exception: " + ex.msg() + "\n";
    }
    return NULL;
}

/*
 * 
 */
int main(int argc, char** argv) {
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
        cerr << "Client thread could not be created.";
        return EXIT_FAILURE;
    }
    if (pthread_join(serverThread, NULL)) {
        cerr << "Could not join server thread.";
        return EXIT_FAILURE;
    }
    if (pthread_join(clientThread, NULL)) {
        cerr << "Could not join client thread.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
