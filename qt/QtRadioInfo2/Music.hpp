/* 
 * File:   Music.hpp
 * Author: zoli
 *
 * Created on 2013. április 4., 0:58
 */

#ifndef MUSIC_HPP
#define	MUSIC_HPP

#include <string>

class Music {
public:
    Music(std::string, std::string);
    std::string artist();
    std::string title();
    std::string text();
private:
    std::string a, t;
};

#endif	/* MUSIC_HPP */
