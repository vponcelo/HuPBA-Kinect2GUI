/****************************************************************************
** Meta object code from reading C++ file 'kinectgui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../kinectgui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kinectgui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KinectGUI_t {
    QByteArrayData data[18];
    char stringdata[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KinectGUI_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KinectGUI_t qt_meta_stringdata_KinectGUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "KinectGUI"
QT_MOC_LITERAL(1, 10, 7), // "resized"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 20), // "startKinectCapturing"
QT_MOC_LITERAL(4, 40, 19), // "stopKinectCapturing"
QT_MOC_LITERAL(5, 60, 20), // "startKinectRecording"
QT_MOC_LITERAL(6, 81, 19), // "stopKinectRecording"
QT_MOC_LITERAL(7, 101, 12), // "updateImages"
QT_MOC_LITERAL(8, 114, 9), // "updateFPS"
QT_MOC_LITERAL(9, 124, 10), // "saveFrames"
QT_MOC_LITERAL(10, 135, 16), // "cv::VideoWriter&"
QT_MOC_LITERAL(11, 152, 2), // "vw"
QT_MOC_LITERAL(12, 155, 8), // "cv::Mat&"
QT_MOC_LITERAL(13, 164, 5), // "image"
QT_MOC_LITERAL(14, 170, 11), // "std::string"
QT_MOC_LITERAL(15, 182, 1), // "s"
QT_MOC_LITERAL(16, 184, 15), // "checkboxChanged"
QT_MOC_LITERAL(17, 200, 15) // "changeImageSize"

    },
    "KinectGUI\0resized\0\0startKinectCapturing\0"
    "stopKinectCapturing\0startKinectRecording\0"
    "stopKinectRecording\0updateImages\0"
    "updateFPS\0saveFrames\0cv::VideoWriter&\0"
    "vw\0cv::Mat&\0image\0std::string\0s\0"
    "checkboxChanged\0changeImageSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KinectGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    3,   71,    2, 0x08 /* Private */,
      16,    1,   78,    2, 0x08 /* Private */,
      17,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 12, 0x80000000 | 14,   11,   13,   15,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void KinectGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KinectGUI *_t = static_cast<KinectGUI *>(_o);
        switch (_id) {
        case 0: _t->resized(); break;
        case 1: _t->startKinectCapturing(); break;
        case 2: _t->stopKinectCapturing(); break;
        case 3: _t->startKinectRecording(); break;
        case 4: _t->stopKinectRecording(); break;
        case 5: _t->updateImages(); break;
        case 6: _t->updateFPS(); break;
        case 7: _t->saveFrames((*reinterpret_cast< cv::VideoWriter(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3]))); break;
        case 8: _t->checkboxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->changeImageSize(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (KinectGUI::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KinectGUI::resized)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject KinectGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_KinectGUI.data,
      qt_meta_data_KinectGUI,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *KinectGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KinectGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
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
void KinectGUI::resized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
