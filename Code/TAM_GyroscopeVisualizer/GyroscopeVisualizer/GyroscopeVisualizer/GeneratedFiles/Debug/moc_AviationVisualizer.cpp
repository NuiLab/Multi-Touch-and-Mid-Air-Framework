/****************************************************************************
** Meta object code from reading C++ file 'AviationVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AviationVisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AviationVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AviationVisualizer_t {
    QByteArrayData data[10];
    char stringdata[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AviationVisualizer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AviationVisualizer_t qt_meta_stringdata_AviationVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "AviationVisualizer"
QT_MOC_LITERAL(1, 19, 7), // "setGyro"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "pitch"
QT_MOC_LITERAL(4, 34, 4), // "roll"
QT_MOC_LITERAL(5, 39, 3), // "yaw"
QT_MOC_LITERAL(6, 43, 5), // "accel"
QT_MOC_LITERAL(7, 49, 2), // "Ax"
QT_MOC_LITERAL(8, 52, 2), // "Ay"
QT_MOC_LITERAL(9, 55, 2) // "Az"

    },
    "AviationVisualizer\0setGyro\0\0pitch\0"
    "roll\0yaw\0accel\0Ax\0Ay\0Az"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AviationVisualizer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    7,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    3,    4,    5,    6,    7,    8,    9,

       0        // eod
};

void AviationVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AviationVisualizer *_t = static_cast<AviationVisualizer *>(_o);
        switch (_id) {
        case 0: _t->setGyro((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7]))); break;
        default: ;
        }
    }
}

const QMetaObject AviationVisualizer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AviationVisualizer.data,
      qt_meta_data_AviationVisualizer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AviationVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AviationVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AviationVisualizer.stringdata))
        return static_cast<void*>(const_cast< AviationVisualizer*>(this));
    return QWidget::qt_metacast(_clname);
}

int AviationVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
