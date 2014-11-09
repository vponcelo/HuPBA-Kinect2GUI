/****************************************************************************
** Meta object code from reading C++ file 'kinectgui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../kinectgui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kinectgui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KinectGUI_t {
    QByteArrayData data[16];
    char stringdata[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KinectGUI_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KinectGUI_t qt_meta_stringdata_KinectGUI = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 20),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 19),
QT_MOC_LITERAL(4, 52, 20),
QT_MOC_LITERAL(5, 73, 19),
QT_MOC_LITERAL(6, 93, 12),
QT_MOC_LITERAL(7, 106, 9),
QT_MOC_LITERAL(8, 116, 10),
QT_MOC_LITERAL(9, 127, 16),
QT_MOC_LITERAL(10, 144, 2),
QT_MOC_LITERAL(11, 147, 8),
QT_MOC_LITERAL(12, 156, 5),
QT_MOC_LITERAL(13, 162, 6),
QT_MOC_LITERAL(14, 169, 1),
QT_MOC_LITERAL(15, 171, 15)
    },
    "KinectGUI\0startKinectCapturing\0\0"
    "stopKinectCapturing\0startKinectRecording\0"
    "stopKinectRecording\0updateImages\0"
    "updateFPS\0saveFrames\0cv::VideoWriter&\0"
    "vw\0cv::Mat&\0image\0string\0s\0checkboxChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KinectGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    3,   60,    2, 0x08 /* Private */,
      15,    1,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11, 0x80000000 | 13,   10,   12,   14,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void KinectGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KinectGUI *_t = static_cast<KinectGUI *>(_o);
        switch (_id) {
        case 0: _t->startKinectCapturing(); break;
        case 1: _t->stopKinectCapturing(); break;
        case 2: _t->startKinectRecording(); break;
        case 3: _t->stopKinectRecording(); break;
        case 4: _t->updateImages(); break;
        case 5: _t->updateFPS(); break;
        case 6: _t->saveFrames((*reinterpret_cast< cv::VideoWriter(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2])),(*reinterpret_cast< string(*)>(_a[3]))); break;
        case 7: _t->checkboxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject KinectGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_KinectGUI.data,
      qt_meta_data_KinectGUI,  qt_static_metacall, 0, 0}
};


const QMetaObject *KinectGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KinectGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KinectGUI.stringdata))
        return static_cast<void*>(const_cast< KinectGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int KinectGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
