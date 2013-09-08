/* 
 * File:   FileUtil.cpp
 * Author: zoli
 * 
 * Created on 2013. szeptember 5., 17:23
 */

#include "FileUtils.h"

#include <unistd.h>
#include <fstream>

std::string FileUtils::path(std::string filename) {
    if (filename.at(0) == '/') {
        return filename;
    }
    char buff[1024];
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1) {
        buff[len] = '\0';
        std::string s(buff);
        return s.substr(0, s.find_last_of("/") + 1).append(filename);
    }
    throw "path error";
}

bool FileUtils::fexists(std::string filename) {
    if (filename.empty()) return false;
    std::ifstream ifile(filename.c_str());
    return ifile;
}
