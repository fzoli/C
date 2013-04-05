/*
 * File:   RadioInfoForm.cpp
 * Author: zoli
 *
 * Created on 2013. április 4., 14:26
 */

#include "RadioInfoForm.h"
#include "RadioInfo.hpp"

#include <iostream>
#include <fstream>

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
        music = i.music();
        std::cout << music->text() + "\n";
        setInfoText(music->artist(), music->title());
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
    widget.actionSave->setEnabled(!err && !prg && !isSaved());
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

bool RadioInfoForm::isSaved() {
    if (music == NULL) return false;
    std::string line;
    std::ifstream is;
    is.open("music.txt");
    bool found = false;
    if (is.is_open()) {
        while (!is.eof()) {
            std::getline(is, line);
            if (line.find(music->text()) < line.npos) {
                found = true;
                break;
            }
        }
    }
    is.close();
    return found;
}

void RadioInfoForm::onSave() {
    if (music == NULL || isSaved()) return;
    std::ofstream os;
    os.open("music.txt", std::ios_base::app);
    os << music->text() + "\r\n";
    os.close();
    widget.actionSave->setEnabled(false);
}
