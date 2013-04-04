/* 
 * File:   RadioInfo.cpp
 * Author: zoli
 * 
 * Created on 2013. április 3., 22:08
 */

#include "RadioInfo.hpp"

#include <sstream>

#include <boost/regex.hpp>

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

RadioInfo::RadioInfo() {
    load_src();
    create_music();
}

bool RadioInfo::success() {
    return !src.empty() && m != NULL;
}

Music* RadioInfo::music() {
    return m;
}

const char* RadioInfo::error() {
    return err;
}

void RadioInfo::load_src() {
    try {
        std::ostringstream os;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(std::string("http://classfm.hu/front_inc/ezek_mennek.php?thispoll=poll")));
        request.setOpt(new curlpp::options::WriteStream(&os));
        request.perform();
        src = os.str();
    }
    catch(curlpp::RuntimeError &e) {
        err = e.what();
    }
}

void RadioInfo::create_music() {
    if (src.empty()) return;
    std::string s = src.substr(src.find("<article"));
    s = s.substr(0, s.find("</article>") + 10);
    boost::regex e("<article.*<a.*>(.*)</a>.*<span.*>(.*)</span>.*</article>");
    boost::match_results<std::string::const_iterator> what;
    if (boost::regex_search(s, what, e)) {
        m = new Music(what[2], what[1]);
    }
}
