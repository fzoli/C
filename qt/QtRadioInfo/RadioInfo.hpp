/* 
 * File:   RadioInfo.hpp
 * Author: zoli
 *
 * Created on 2013. április 3., 22:08
 */

#ifndef RADIOINFO_HPP
#define	RADIOINFO_HPP

#include <string>

#include "Music.hpp"

class RadioInfo {
public:
    RadioInfo();
    bool success();
    Music* music();
    std::string error();
private:
    Music* m;
    const char* err;
    std::string src;
    void load_src();
    void create_music();
};

#endif	/* RADIOINFO_HPP */
