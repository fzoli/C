/* 
 * File:   RadioInfo.cpp
 * Author: zoli
 * 
 * Created on 2013. április 3., 22:08
 */

#include <QtCore/QRegExp>

#include "RadioInfo.hpp"

RadioInfo::RadioInfo(std::string s) {
    src = s;
    create_music();
}

bool RadioInfo::success() {
    return !src.empty() && m != NULL;
}

Music* RadioInfo::music() {
    return m;
}

void RadioInfo::create_music() {
    if (src.empty()) return;
    std::string s = src.substr(src.find("<article"));
    s = s.substr(0, s.find("</article>") + 10);
    QRegExp rx("<article.*<a.*>(.*)</a>.*<span.*>(.*)</span>.*</article>");
    rx.indexIn(s.c_str());
    if (rx.captureCount() > 0)
        m = new Music(rx.cap(2).toStdString(), rx.cap(1).toStdString());
    else
        m = NULL;
}
