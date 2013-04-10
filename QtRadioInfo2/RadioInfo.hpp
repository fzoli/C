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
    RadioInfo(std::string);
    bool success();
    Music* music();
private:
    Music* m;
    std::string src;
    void create_music();
};

#endif	/* RADIOINFO_HPP */
