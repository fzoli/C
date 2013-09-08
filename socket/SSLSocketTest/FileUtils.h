/* 
 * File:   FileUtil.h
 * Author: zoli
 *
 * Created on 2013. szeptember 5., 17:23
 */

#ifndef FILEUTIL_H
#define	FILEUTIL_H

#include <string>

namespace FileUtils {
    
    std::string path(std::string filename);
    
    bool fexists(std::string filename);
    
}

#endif	/* FILEUTIL_H */
