/* 
 * File:   Music.cpp
 * Author: zoli
 * 
 * Created on 2013. április 4., 0:58
 */

#include "Music.hpp"

Music::Music(std::string a, std::string t) {
    Music::a = a;
    Music::t = t;
}

std::string Music::artist() {
    return a.substr(0, a.rfind(" (most megy)"));
}

std::string Music::title() {
    return t;
}

std::string Music::text() {
    return artist() + " - " + title();
}
