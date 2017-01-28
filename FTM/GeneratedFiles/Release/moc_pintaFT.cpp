/****************************************************************************
** Meta object code from reading C++ file 'pintaFT.h'
**
** Created: Thu 6. Oct 14:02:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pintaFT.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pintaFT.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_pintaFT[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,    9,    8,    8, 0x05,
      38,    8,    8,    8, 0x05,
      50,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,    8,    8,    8, 0x0a,
      73,    9,    8,    8, 0x0a,
     110,  102,    8,    8, 0x0a,
     136,  132,    8,    8, 0x0a,
     159,  132,    8,    8, 0x0a,
     201,  179,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_pintaFT[] = {
    "pintaFT\0\0Fig\0cambioFig(clasificaFig*)\0"
    "updateVol()\0creaFig()\0Selecciono()\0"
    "actualizarFTM(clasificaFig*)\0fil,col\0"
    "creaComboBox(int,int)\0fig\0"
    "actulizarFigClasi(int)\0activaFigClasi(int)\0"
    "fil,col,filAnt,colAnt\0"
    "cambioCellSelect(int,int,int,int)\0"
};

const QMetaObject pintaFT::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_pintaFT,
      qt_meta_data_pintaFT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &pintaFT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *pintaFT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *pintaFT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pintaFT))
        return static_cast<void*>(const_cast< pintaFT*>(this));
    return QWidget::qt_metacast(_clname);
}

int pintaFT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cambioFig((*reinterpret_cast< clasificaFig*(*)>(_a[1]))); break;
        case 1: updateVol(); break;
        case 2: creaFig(); break;
        case 3: Selecciono(); break;
        case 4: actualizarFTM((*reinterpret_cast< clasificaFig*(*)>(_a[1]))); break;
        case 5: creaComboBox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: actulizarFigClasi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: activaFigClasi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: cambioCellSelect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void pintaFT::cambioFig(clasificaFig * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void pintaFT::updateVol()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void pintaFT::creaFig()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
