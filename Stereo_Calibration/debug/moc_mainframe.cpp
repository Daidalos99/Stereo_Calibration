/****************************************************************************
** Meta object code from reading C++ file 'mainframe.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainframe.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainFrame_t {
    uint offsetsAndSizes[26];
    char stringdata0[10];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[32];
    char stringdata4[28];
    char stringdata5[6];
    char stringdata6[26];
    char stringdata7[17];
    char stringdata8[22];
    char stringdata9[30];
    char stringdata10[24];
    char stringdata11[37];
    char stringdata12[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainFrame_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainFrame_t qt_meta_stringdata_MainFrame = {
    {
        QT_MOC_LITERAL(0, 9),  // "MainFrame"
        QT_MOC_LITERAL(10, 21),  // "on_buttonOpen_clicked"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 31),  // "on_buttonDeleteContents_clicked"
        QT_MOC_LITERAL(65, 27),  // "on_tabWidget_currentChanged"
        QT_MOC_LITERAL(93, 5),  // "index"
        QT_MOC_LITERAL(99, 25),  // "on_buttonShowList_clicked"
        QT_MOC_LITERAL(125, 16),  // "on_zhang_clicked"
        QT_MOC_LITERAL(142, 21),  // "on_plot_point_clicked"
        QT_MOC_LITERAL(164, 29),  // "on_stereo_calibration_clicked"
    },
    "MainFrame",
    "on_buttonOpen_clicked",
    "",
    "on_buttonDeleteContents_clicked",
    "on_tabWidget_currentChanged",
    "index",
    "on_buttonShowList_clicked",
    "on_zhang_clicked",
    "on_plot_point_clicked",
    "on_stereo_calibration_clicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainFrame[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    1 /* Private */,
       3,    0,   75,    2, 0x08,    2 /* Private */,
       4,    1,   76,    2, 0x08,    3 /* Private */,
       6,    0,   79,    2, 0x08,    5 /* Private */,
       7,    0,   80,    2, 0x08,    6 /* Private */,
       8,    0,   81,    2, 0x08,    7 /* Private */,
       9,    0,   82,    2, 0x08,    8 /* Private */,
      10,    0,   83,    2, 0x08,    9 /* Private */,
      11,    0,   84,    2, 0x08,   10 /* Private */,
      12,    0,   85,    2, 0x08,   11 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainFrame::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MainFrame.offsetsAndSizes,
    qt_meta_data_MainFrame,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainFrame_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainFrame, std::true_type>,
        // method 'on_buttonOpen_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_buttonDeleteContents_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_buttonShowList_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_zhang_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_plot_point_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_stereo_calibration_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainFrame *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_buttonOpen_clicked(); break;
        case 1: _t->on_buttonDeleteContents_clicked(); break;
        case 2: _t->on_tabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_buttonShowList_clicked(); break;
        case 4: _t->on_zhang_clicked(); break;
        case 5: _t->on_plot_point_clicked(); break;
        case 6: _t->on_stereo_calibration_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrame.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MainFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
