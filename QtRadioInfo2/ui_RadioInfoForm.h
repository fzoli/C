/********************************************************************************
** Form generated from reading UI file 'RadioInfoForm.ui'
**
** Created: Wed May 15 19:03:21 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADIOINFOFORM_H
#define UI_RADIOINFOFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RadioInfoForm
{
public:
    QAction *actionSave;
    QAction *actionRefresh;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *lbArtist;
    QLabel *label2;
    QLabel *lbTitle;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RadioInfoForm)
    {
        if (RadioInfoForm->objectName().isEmpty())
            RadioInfoForm->setObjectName(QString::fromUtf8("RadioInfoForm"));
        RadioInfoForm->resize(200, 118);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/ClassFM-logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        RadioInfoForm->setWindowIcon(icon);
        actionSave = new QAction(RadioInfoForm);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setCheckable(false);
        actionSave->setChecked(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/Save-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionRefresh = new QAction(RadioInfoForm);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/Refresh-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRefresh->setIcon(icon2);
        centralWidget = new QWidget(RadioInfoForm);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lbArtist = new QLabel(centralWidget);
        lbArtist->setObjectName(QString::fromUtf8("lbArtist"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbArtist->sizePolicy().hasHeightForWidth());
        lbArtist->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(lbArtist, 0, 1, 1, 1);

        label2 = new QLabel(centralWidget);
        label2->setObjectName(QString::fromUtf8("label2"));
        sizePolicy.setHeightForWidth(label2->sizePolicy().hasHeightForWidth());
        label2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label2, 1, 0, 1, 1);

        lbTitle = new QLabel(centralWidget);
        lbTitle->setObjectName(QString::fromUtf8("lbTitle"));
        sizePolicy1.setHeightForWidth(lbTitle->sizePolicy().hasHeightForWidth());
        lbTitle->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(lbTitle, 1, 1, 1, 1);

        RadioInfoForm->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(RadioInfoForm);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setContextMenuPolicy(Qt::NoContextMenu);
        mainToolBar->setMovable(true);
        mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainToolBar->setFloatable(true);
        RadioInfoForm->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RadioInfoForm);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RadioInfoForm->setStatusBar(statusBar);

        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRefresh);

        retranslateUi(RadioInfoForm);

        QMetaObject::connectSlotsByName(RadioInfoForm);
    } // setupUi

    void retranslateUi(QMainWindow *RadioInfoForm)
    {
        RadioInfoForm->setWindowTitle(QApplication::translate("RadioInfoForm", "R\303\241di\303\263 inf\303\263", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("RadioInfoForm", "Ment\303\251s", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("RadioInfoForm", "Elmenti a dalt a txt \303\241llom\303\241nyba.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("RadioInfoForm", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionRefresh->setText(QApplication::translate("RadioInfoForm", "Friss\303\255t\303\251s", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRefresh->setToolTip(QApplication::translate("RadioInfoForm", "Friss\303\255ti az adatokat.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRefresh->setShortcut(QApplication::translate("RadioInfoForm", "F5", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RadioInfoForm", "El\305\221ad\303\263:", 0, QApplication::UnicodeUTF8));
        lbArtist->setText(QApplication::translate("RadioInfoForm", "-", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("RadioInfoForm", "C\303\255m:", 0, QApplication::UnicodeUTF8));
        lbTitle->setText(QApplication::translate("RadioInfoForm", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RadioInfoForm: public Ui_RadioInfoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADIOINFOFORM_H
