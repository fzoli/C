/********************************************************************************
** Form generated from reading UI file 'HelloForm.ui'
**
** Created: Sun Mar 31 14:54:01 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELLOFORM_H
#define UI_HELLOFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HelloForm
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *msgLabel;
    QLineEdit *nameEdit;
    QLabel *nameLabel;

    void setupUi(QDialog *HelloForm)
    {
        if (HelloForm->objectName().isEmpty())
            HelloForm->setObjectName(QString::fromUtf8("HelloForm"));
        HelloForm->resize(451, 109);
        verticalLayout = new QVBoxLayout(HelloForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        msgLabel = new QLabel(HelloForm);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));

        verticalLayout->addWidget(msgLabel);

        nameEdit = new QLineEdit(HelloForm);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        verticalLayout->addWidget(nameEdit);

        nameLabel = new QLabel(HelloForm);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setStyleSheet(QString::fromUtf8("background:rgb(255, 255, 255)"));

        verticalLayout->addWidget(nameLabel);


        retranslateUi(HelloForm);

        QMetaObject::connectSlotsByName(HelloForm);
    } // setupUi

    void retranslateUi(QDialog *HelloForm)
    {
        HelloForm->setWindowTitle(QApplication::translate("HelloForm", "Hell\303\263", 0, QApplication::UnicodeUTF8));
        msgLabel->setText(QApplication::translate("HelloForm", "K\303\251rem, adja meg a nev\303\251t:", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HelloForm: public Ui_HelloForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOFORM_H
