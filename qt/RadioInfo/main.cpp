/* 
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2013. április 3., 3:20
 */

#include <iostream>

#include "RadioInfo.hpp"

/*
 * HTTP GET method testing by cURLpp and regex testing by boost.
 */
int main(int argc, char** argv) {
    RadioInfo i;
    if (i.success()) {
        Music* m = i.music();
        std::cout << m->artist() + " — " + m->title() + "\n";
        return 0;
    }
    else {
        std::cout << i.error();
        return 1;
    }
}
