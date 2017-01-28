/****************************************************************************
** Meta object code from reading C++ file 'pantalla.h'
**
** Created: Thu 6. Oct 14:02:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pantalla.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pantalla.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_pantalla[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      29,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_pantalla[] = {
    "pantalla\0\0updateVolumenFTM()\0loadFTUni()\0"
};

const QMetaObject pantalla::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_pantalla,
      qt_meta_data_pantalla, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &pantalla::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *pantalla::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *pantalla::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pantalla))
        return static_cast<void*>(const_cast< pantalla*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int pantalla::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateVolumenFTM(); break;
        case 1: loadFTUni(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
