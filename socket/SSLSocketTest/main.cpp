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
        
        Socket* c = s.accept();
        
        ostream os(c->getBuffer());
        os << "Hello" << " " << "World!";
        
//        c->read();
        
        c->close();
        s.close();
        delete c;
    }
    catch (CertificateException &ex) {
        cerr << "Certificate exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SSLSocketException &ex) {
        cerr << "SSL Server Socket exception: " + ex.msg() + "\n";
        waitServer = -1;
    }
    catch (SocketException &ex) {
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
        while (is.good() && !c.isClosed()) {
            string line;
            std::getline(is, line);
            if (!line.empty()) cout << line + "\n";
        }
        
//        string reply;
//        c >> reply;
//        cout << reply + "\n";
        
        c.close();
    }
    catch (CertificateException &ex) {
        cerr << "Certificate exception: " + ex.msg() + "\n";
    }
    catch (SSLSocketException &ex) {
        cerr << "SSL Socket exception: " + ex.msg() + "\n";
    }
    catch (SocketException &ex) {
        cerr << "Socket exception: " + ex.msg() + "\n";
    }
    pthread_exit(NULL);
}

/*
 * 
 */
int main(int argc, char** argv) {
    pthread_t threads;
    if (pthread_create(&threads, NULL, &server, NULL)) {
        cerr << "Server thread could not be created.";
        return EXIT_FAILURE;
    }
    while(waitServer == 1) {
        usleep(1);
    }
    if (waitServer == -1) {
        return EXIT_FAILURE;
    }
    if (pthread_create(&threads, NULL, &client, NULL)) {
        cerr << "Client thread could not be created.\n";
        return EXIT_FAILURE;
    }
    if (pthread_join(threads, NULL)) {
        cerr << "Could not join threads.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
