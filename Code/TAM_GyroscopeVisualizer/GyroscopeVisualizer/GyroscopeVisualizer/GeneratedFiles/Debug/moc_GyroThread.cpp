/****************************************************************************
** Meta object code from reading C++ file 'GyroThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GyroThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GyroThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GyroThread_t {
    QByteArrayData data[17];
    char stringdata[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GyroThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GyroThread_t qt_meta_stringdata_GyroThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GyroThread"
QT_MOC_LITERAL(1, 11, 8), // "gyroData"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "pitch"
QT_MOC_LITERAL(4, 27, 4), // "roll"
QT_MOC_LITERAL(5, 32, 3), // "yaw"
QT_MOC_LITERAL(6, 36, 5), // "accel"
QT_MOC_LITERAL(7, 42, 2), // "Ax"
QT_MOC_LITERAL(8, 45, 2), // "Ay"
QT_MOC_LITERAL(9, 48, 2), // "Az"
QT_MOC_LITERAL(10, 51, 6), // "GLData"
QT_MOC_LITERAL(11, 58, 1), // "x"
QT_MOC_LITERAL(12, 60, 1), // "y"
QT_MOC_LITERAL(13, 62, 1), // "z"
QT_MOC_LITERAL(14, 64, 1), // "w"
QT_MOC_LITERAL(15, 66, 9), // "sendError"
QT_MOC_LITERAL(16, 76, 3) // "msg"

    },
    "GyroThread\0gyroData\0\0pitch\0roll\0yaw\0"
    "accel\0Ax\0Ay\0Az\0GLData\0x\0y\0z\0w\0sendError\0"
    "msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GyroThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    7,   29,    2, 0x06 /* Public */,
      10,    4,   44,    2, 0x06 /* Public */,
      15,    1,   53,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    3,    4,    5,    6,    7,    8,    9,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QString,   16,

       0        // eod
};

void GyroThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GyroThread *_t = static_cast<GyroThread *>(_o);
        switch (_id) {
        case 0: _t->gyroData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7]))); break;
        case 1: _t->GLData((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 2: _t->sendError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GyroThread::*_t)(double , double , double , double , double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GyroThread::gyroData)) {
                *result = 0;
            }
        }
        {
            typedef void (GyroThread::*_t)(float , float , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GyroThread::GLData)) {
                *result = 1;
            }
        }
        {
            typedef void (GyroThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GyroThread::sendError)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GyroThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_GyroThread.data,
      qt_meta_data_GyroThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GyroThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GyroThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GyroThread.stringdata))
        return static_cast<void*>(const_cast< GyroThread*>(this));
    return QThread::qt_metacast(_clname);
}

int GyroThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GyroThread::gyroData(double _t1, double _t2, double _t3, double _t4, double _t5, double _t6, double _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GyroThread::GLData(float _t1, float _t2, float _t3, float _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GyroThread::sendError(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
