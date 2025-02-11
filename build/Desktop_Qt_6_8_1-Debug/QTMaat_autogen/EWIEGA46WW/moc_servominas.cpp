/****************************************************************************
** Meta object code from reading C++ file 'servominas.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../servominas.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'servominas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10ServoMinasE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10ServoMinasE = QtMocHelpers::stringData(
    "ServoMinas",
    "logMessage",
    "",
    "message",
    "inputChangedSignal",
    "minas_control::MinasInput",
    "input",
    "initializationFinished",
    "situation",
    "operationStatusSignal",
    "status",
    "updateCommunicationState",
    "checked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10ServoMinasE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       4,    1,   47,    2, 0x06,    3 /* Public */,
       7,    1,   50,    2, 0x06,    5 /* Public */,
       9,    1,   53,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,   56,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject ServoMinas::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN10ServoMinasE.offsetsAndSizes,
    qt_meta_data_ZN10ServoMinasE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10ServoMinasE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ServoMinas, std::true_type>,
        // method 'logMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'inputChangedSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<minas_control::MinasInput, std::false_type>,
        // method 'initializationFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'operationStatusSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateCommunicationState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void ServoMinas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ServoMinas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->inputChangedSignal((*reinterpret_cast< std::add_pointer_t<minas_control::MinasInput>>(_a[1]))); break;
        case 2: _t->initializationFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->operationStatusSignal((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->updateCommunicationState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ServoMinas::*)(QString );
            if (_q_method_type _q_method = &ServoMinas::logMessage; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (ServoMinas::*)(minas_control::MinasInput );
            if (_q_method_type _q_method = &ServoMinas::inputChangedSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (ServoMinas::*)(bool );
            if (_q_method_type _q_method = &ServoMinas::initializationFinished; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (ServoMinas::*)(bool );
            if (_q_method_type _q_method = &ServoMinas::operationStatusSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ServoMinas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServoMinas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10ServoMinasE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ServoMinas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ServoMinas::logMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServoMinas::inputChangedSignal(minas_control::MinasInput _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ServoMinas::initializationFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ServoMinas::operationStatusSignal(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
