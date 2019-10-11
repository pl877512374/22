/****************************************************************************
** Meta object code from reading C++ file 'workthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../workthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'workthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_workthread_t {
    QByteArrayData data[12];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_workthread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_workthread_t qt_meta_stringdata_workthread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "workthread"
QT_MOC_LITERAL(1, 11, 8), // "Draw_sig"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 15), // "DetectTable_sig"
QT_MOC_LITERAL(4, 37, 15), // "TcpConnInfo_sig"
QT_MOC_LITERAL(5, 53, 12), // "InfoView_sig"
QT_MOC_LITERAL(6, 66, 9), // "Enablebtn"
QT_MOC_LITERAL(7, 76, 9), // "Draw_slot"
QT_MOC_LITERAL(8, 86, 16), // "DetectTable_slot"
QT_MOC_LITERAL(9, 103, 16), // "TcpConnInfo_slot"
QT_MOC_LITERAL(10, 120, 13), // "InfoView_slot"
QT_MOC_LITERAL(11, 134, 14) // "Enablebtn_slot"

    },
    "workthread\0Draw_sig\0\0DetectTable_sig\0"
    "TcpConnInfo_sig\0InfoView_sig\0Enablebtn\0"
    "Draw_slot\0DetectTable_slot\0TcpConnInfo_slot\0"
    "InfoView_slot\0Enablebtn_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_workthread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,
       4,    1,   70,    2, 0x06 /* Public */,
       5,    1,   73,    2, 0x06 /* Public */,
       6,    0,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   77,    2, 0x0a /* Public */,
       8,    1,   80,    2, 0x0a /* Public */,
       9,    1,   83,    2, 0x0a /* Public */,
      10,    1,   86,    2, 0x0a /* Public */,
      11,    0,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void workthread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        workthread *_t = static_cast<workthread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Draw_sig((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->DetectTable_sig((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->TcpConnInfo_sig((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->InfoView_sig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->Enablebtn(); break;
        case 5: _t->Draw_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->DetectTable_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->TcpConnInfo_slot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->InfoView_slot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->Enablebtn_slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (workthread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&workthread::Draw_sig)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (workthread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&workthread::DetectTable_sig)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (workthread::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&workthread::TcpConnInfo_sig)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (workthread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&workthread::InfoView_sig)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (workthread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&workthread::Enablebtn)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject workthread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_workthread.data,
      qt_meta_data_workthread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *workthread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *workthread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_workthread.stringdata0))
        return static_cast<void*>(const_cast< workthread*>(this));
    return QThread::qt_metacast(_clname);
}

int workthread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void workthread::Draw_sig(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void workthread::DetectTable_sig(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void workthread::TcpConnInfo_sig(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void workthread::InfoView_sig(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void workthread::Enablebtn()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
