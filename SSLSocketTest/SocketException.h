#ifndef SocketException_class
#define SocketException_class

#include <string>
#include <stdexcept>

class SocketException : public std::runtime_error {
    
  public:

    SocketException(std::string s="") : std::runtime_error(s) {}
  
    std::string msg() {
      std::string desc(what());
      return desc;
    }
  
};

#endif
