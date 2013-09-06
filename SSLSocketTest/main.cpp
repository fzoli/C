/* 
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2013. szeptember 1., 19:59
 */

#include <iostream>
#include <unistd.h>

#include "SSLServerSocket.h"
#include "CertificateException.h"
#include "SocketBuffer.h"
#include "FileUtils.h"

#define PORT 9443

using namespace std;
using namespace FileUtils;

int waitServer = 1;

void* server(void*) {
    string CAfile = path("ca.crt");
    string CRTfile = path("server.crt");
    string KEYfile = path("server.key");
    char *KEYpass = (char *) "asdfgh";
    try {
//        ServerSocket s(PORT);
        SSLServerSocket s(PORT, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass);
        s.setTimeout(1);
        waitServer = 0;
        
//        Socket c = s.accept();
        SSLSocket c = s.accept();
        
        c << "Hello" << " ";
        ostream os(c.getBuffer());
        os << "World!";
        
//        c.read();
        
        c.close();
        s.close();
    }
    catch (CertificateException ex) {
        cerr << "Certificate exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SSLSocketException ex) {
        cerr << "SSL Server Socket exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SocketException ex) {
        cerr << "Socket Server exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    pthread_exit(NULL);
}

void* client(void*) {
    string CAfile = path("ca.crt");
    string CRTfile = path("client.crt");
    string KEYfile = path("client.key");
    char *KEYpass = (char *) "asdfgh";
    try {
//        Socket c("localhost", PORT, 1);
        SSLSocket c("localhost", PORT, CAfile.c_str(), CRTfile.c_str(), KEYfile.c_str(), KEYpass, 1);
        c.setTimeout(1);
        
//        c.write(1);
        
        istream is(c.getBuffer());
        while (!c.isClosed()) {
            string line;
            std::getline(is, line);
            if (!line.empty()) cout << line + "\n";
        }
        
//        string reply;
//        c >> reply;
//        cout << reply + "\n";
        
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
    pthread_exit(NULL);
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
