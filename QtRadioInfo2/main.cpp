/*
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2013. április 4., 13:21
 */

#include <QtGui/QApplication>

#include "RadioInfoForm.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    RadioInfoForm form;
    form.show();
    
    return app.exec();
}
