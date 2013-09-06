/*
 * File:   HelloForm.cpp
 * Author: zoli
 *
 * Created on 2012. április 24., 1:11
 */

#include "HelloForm.h"

HelloForm::HelloForm() {
    widget.setupUi(this);
    connect(widget.nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
}

HelloForm::~HelloForm() {
}

void HelloForm::textChanged(const QString& text) {
    if (0 < text.trimmed().length()) {
        widget.nameLabel->setText("Szia " + text.trimmed() + "!");
    }
    else {
        widget.nameLabel->clear();
    }
}