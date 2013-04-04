/*
 * File:   RadioInfoForm.cpp
 * Author: zoli
 *
 * Created on 2013. április 4., 14:26
 */

#include "RadioInfoForm.h"
#include "RadioInfo.hpp"

#include <iostream>

#include <boost/thread.hpp>

RadioInfoForm::RadioInfoForm() {
    widget.setupUi(this);
    connect(widget.actionSave, SIGNAL(activated()), this, SLOT(onSave()));
    connect(widget.actionRefresh, SIGNAL(activated()), this, SLOT(onRefresh()));
    connect(this, SIGNAL(statusChange(const char*, bool)), this, SLOT(onStatusChanged(const char*, bool)));
    load();
}

RadioInfoForm::~RadioInfoForm() {
}

void RadioInfoForm::load() {
    setMessage("Betöltés folyamatban...", false, true);
    boost::thread(boost::bind(&RadioInfoForm::t_load, this));
}

void RadioInfoForm::t_load() {
    RadioInfo i;
    if (i.success()) {
        Music* m = i.music();
        std::cout << m->text() + "\n";
        setInfoText(m->artist(), m->title());
        emit statusChange("Sikeres betöltés.", false);
    }
    else {
        std::cerr << i.error() + "\n";
        emit statusChange(i.error().c_str(), true);
    }
}

void RadioInfoForm::setMessage(const char* m, bool err) {
    setMessage(m, err, false);
}

void RadioInfoForm::setMessage(const char* m, bool err, bool prg) {
    statusBar()->showMessage((err ? "Hiba: " : "") + QString::fromUtf8(m));
    widget.centralWidget->setVisible(!err && !prg);
    widget.actionRefresh->setEnabled(!prg);
    widget.actionSave->setEnabled(!prg);
}

void RadioInfoForm::setInfoText(std::string a, std::string t) {
    widget.lbArtist->setText(QString::fromStdString(a));
    widget.lbTitle->setText(QString::fromStdString(t));
}

void RadioInfoForm::onStatusChanged(const char* s, bool err) {
    setMessage(s, err);
}

void RadioInfoForm::onRefresh() {
    load();
}

void RadioInfoForm::onSave() {
    
}
