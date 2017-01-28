/****************************************************************************
** Meta object code from reading C++ file 'pintaftseg.h'
**
** Created: Thu 6. Oct 14:02:54 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pintaftseg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pintaftseg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_pintaFTSeg[] = {

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
      16,   12,   11,   11, 0x05,
      44,   11,   11,   11, 0x05,
      56,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   11,   11,   11, 0x0a,
      89,   12,   11,   11, 0x0a,
     125,  121,   11,   11, 0x0a,
     148,  121,   11,   11, 0x0a,
     176,  168,   11,   11, 0x0a,
     220,  198,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_pintaFTSeg[] = {
    "pintaFTSeg\0\0Fig\0cambioFigSeg(clasificaFig*)\0"
    "updateVol()\0creaFig()\0SeleccionoFigOFTable()\0"
    "actualizarFTMSeg(clasificaFig*)\0fig\0"
    "actulizarFigClasi(int)\0activaFigClasi(int)\0"
    "fil,col\0creaComboBox(int,int)\0"
    "fil,col,filAnt,colAnt\0"
    "cambioCellSelect(int,int,int,int)\0"
};

const QMetaObject pintaFTSeg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_pintaFTSeg,
      qt_meta_data_pintaFTSeg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &pintaFTSeg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *pintaFTSeg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *pintaFTSeg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pintaFTSeg))
        return static_cast<void*>(const_cast< pintaFTSeg*>(this));
    return QWidget::qt_metacast(_clname);
}

int pintaFTSeg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cambioFigSeg((*reinterpret_cast< clasificaFig*(*)>(_a[1]))); break;
        case 1: updateVol(); break;
        case 2: creaFig(); break;
        case 3: SeleccionoFigOFTable(); break;
        case 4: actualizarFTMSeg((*reinterpret_cast< clasificaFig*(*)>(_a[1]))); break;
        case 5: actulizarFigClasi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: activaFigClasi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: creaComboBox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: cambioCellSelect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void pintaFTSeg::cambioFigSeg(clasificaFig * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void pintaFTSeg::updateVol()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void pintaFTSeg::creaFig()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
