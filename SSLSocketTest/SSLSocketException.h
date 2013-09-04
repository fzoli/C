#ifndef SSLSocketException_class
#define SSLSocketException_class

#include <string>
#include <stdexcept>

#include "SocketException.h"

class SSLSocketException : public SocketException {
    
  public:
      
    SSLSocketException(std::string s="") : SocketException(s) {}
    
};

#endif
