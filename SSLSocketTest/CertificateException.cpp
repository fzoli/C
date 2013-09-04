/* 
 * File:   CertificateException.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 4., 16:45
 */

#include "CertificateException.h"

CertificateException::CertificateException(std::string msg="") : SSLSocketException(msg) {
    ;
}
