/* 
 * File:   HelloForm.h
 * Author: zoli
 *
 * Created on 2012. április 24., 1:11
 */

#ifndef _HELLOFORM_H
#define	_HELLOFORM_H

#include "ui_HelloForm.h"

class HelloForm : public QDialog {
    Q_OBJECT
public:
    HelloForm();
    virtual ~HelloForm();
public slots:
    void textChanged(const QString& text);
private:
    Ui::HelloForm widget;
};

#endif	/* _HELLOFORM_H */
