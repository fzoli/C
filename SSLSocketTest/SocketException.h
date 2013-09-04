/* 
 * File:   SocketException.h
 * Author: zoli
 *
 * Created on 2013. szeptember 4., 16:11
 */

#ifndef SOCKETEXCEPTION_H
#define	SOCKETEXCEPTION_H

#include <string>
#include <stdexcept>

class SocketException : public std::runtime_error {
    
  public:

    SocketException(std::string msg);
  
    std::string msg();
  
};

#endif	/* SOCKETEXCEPTION_H */
