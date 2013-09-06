/****************************************************************************
** Meta object code from reading C++ file 'RadioInfoForm.h'
**
** Created: Wed Jun 5 16:12:25 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RadioInfoForm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadioInfoForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadioInfoForm[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      24,   14,   14,   14, 0x08,
      36,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RadioInfoForm[] = {
    "RadioInfoForm\0\0onSave()\0onRefresh()\0"
    "requestReceived(QNetworkReply*)\0"
};

const QMetaObject RadioInfoForm::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RadioInfoForm,
      qt_meta_data_RadioInfoForm, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadioInfoForm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadioInfoForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadioInfoForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadioInfoForm))
        return static_cast<void*>(const_cast< RadioInfoForm*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RadioInfoForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onSave(); break;
        case 1: onRefresh(); break;
        case 2: requestReceived((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
