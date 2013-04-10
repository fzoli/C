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

#include <qt4/QtCore/QUrl>
#include <qt4/QtNetwork/QNetworkReply>
#include <qt4/QtNetwork/QNetworkRequest>

RadioInfoForm::RadioInfoForm() {
    widget.setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(widget.actionSave, SIGNAL(activated()), this, SLOT(onSave()));
    connect(widget.actionRefresh, SIGNAL(activated()), this, SLOT(onRefresh()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestReceived(QNetworkReply*)));
    load();
}

RadioInfoForm::~RadioInfoForm() {
}

void RadioInfoForm::load() {
    setMessage("Betöltés folyamatban...", false, true);
    manager->get(QNetworkRequest(QUrl("http://classfm.hu/front_inc/ezek_mennek.php?thispoll=poll")));
}

void RadioInfoForm::load(std::string src) {
    RadioInfo i(src);
    if (i.success()) {
        music = i.music();
        setInfoText(music->artist(), music->title());
        setMessage("Sikeres betöltés.", false);
    }
    else {
        setMessage("A weboldal HTML kódja megváltozott.", true);
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
    if (music == NULL) {
        return;
    }
    if (isSaved()) {
        widget.actionSave->setEnabled(false);
        return;
    }
    std::ofstream os;
    os.open("music.txt", std::ios_base::app);
    os << music->text() + "\r\n";
    if (os.good()) widget.actionSave->setEnabled(false);
    else setMessage("Sikertelen mentés!", false, false);
    os.close();
}

void RadioInfoForm::requestReceived(QNetworkReply* reply) {
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError) {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) {
            QString replyText = reply->readAll();
            load(replyText.toStdString());
        }
        else {
            setMessage("A weboldal nem szolgált adattal.", true);
        }
    }
    else {
        setMessage("A weboldal nem érhető el.", true);
    }
}
