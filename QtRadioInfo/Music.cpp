/* 
 * File:   Music.cpp
 * Author: zoli
 * 
 * Created on 2013. április 4., 0:58
 */

#include "Music.hpp"

#include <qt4/QtCore/qstring.h>

Music::Music(std::string a, std::string t) {
    Music::a = a;
    Music::t = t;
}

std::string Music::artist() {
    return QString::fromStdString(a.substr(0, a.rfind(" (most megy)"))).toStdString();
}

std::string Music::title() {
    return QString::fromStdString(t).toStdString();
}

std::string Music::text() {
    return artist() + " — " + title();
}
