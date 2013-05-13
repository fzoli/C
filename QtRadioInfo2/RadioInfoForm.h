/* 
 * File:   RadioInfoForm.h
 * Author: zoli
 *
 * Created on 2013. április 4., 14:26
 */

#ifndef _RADIOINFOFORM_H
#define	_RADIOINFOFORM_H

#include <string>

#include <QtCore/QDir>
#include <QtNetwork/QNetworkAccessManager>

#include "Music.hpp"

#include "ui_RadioInfoForm.h"

class RadioInfoForm : public QMainWindow {
    Q_OBJECT
public:
    RadioInfoForm();
    virtual ~RadioInfoForm();
signals:
    void statusChange(const char*, bool);
private slots:
    void onSave();
    void onRefresh();
    void requestReceived(QNetworkReply*);
private:
    Ui::RadioInfoForm widget;
    QNetworkAccessManager* manager;
    QDir* appDir;
    Music* music;
    void load();
    void load(std::string);
    bool isSaved();
    void setMessage(const char*, bool);
    void setMessage(const char*, bool, bool);
    void setInfoText(std::string, std::string);
    const char* getMusicFilePath();
};

#endif	/* _RADIOINFOFORM_H */
