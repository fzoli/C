/*
 * File:   main.cpp
 * Author: zoli
 *
 * Created on 2012. április 24., 1:10
 */

#include <QtGui/QApplication>
#include "HelloForm.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    HelloForm form;
    form.show();

    return app.exec();
}
