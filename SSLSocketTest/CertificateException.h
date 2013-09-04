/* 
 * File:   CertificateException.h
 * Author: zoli
 *
 * Created on 2013. szeptember 4., 16:45
 */

#ifndef CERTIFICATEEXCEPTION_H
#define	CERTIFICATEEXCEPTION_H

#include "SSLSocketException.h"

class CertificateException : public SSLSocketException {

  public:

    CertificateException(std::string msg);

};

#endif	/* CERTIFICATEEXCEPTION_H */
