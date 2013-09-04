/* 
 * File:   SocketException.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 4., 16:11
 */

#include "SocketException.h"

SocketException::SocketException(std::string msg="") : std::runtime_error(msg) {
    ;
}

std::string SocketException::msg() {
  std::string desc(what());
  return desc;
}
