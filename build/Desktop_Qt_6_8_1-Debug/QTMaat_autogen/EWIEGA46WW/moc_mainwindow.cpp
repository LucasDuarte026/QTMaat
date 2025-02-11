/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10MainWindowE = QtMocHelpers::stringData(
    "MainWindow",
    "servoCheckBoxChanged",
    "",
    "checked",
    "initSignal",
    "stopOperationSignal",
    "moveServoToPositionSignal",
    "position",
    "velocity",
    "startHomingSignal",
    "resetErrorsSignal",
    "jogSignal",
    "amount",
    "step",
    "readInputNowSignal",
    "updateSensorDependencies",
    "SensorData*",
    "_sensorData",
    "by_sensorSelected_action",
    "on_actionVer_Sensores_triggered",
    "on_actionAdicionar_triggered",
    "on_actionAtualizar_triggered",
    "on_actionRemover_triggered",
    "on_actionLogout_triggered",
    "on_login_menuBar_triggered",
    "on_actionVer_usu_rios_triggered",
    "on_actionAdicionar_2_triggered",
    "on_actionAtualizar_2_triggered",
    "on_actionRemover_2_triggered",
    "by_animate_dial_button_action",
    "servoCommunicationBox_stateChanged",
    "servoState",
    "servoSituation",
    "stopOperation",
    "initializeServo",
    "startHoming",
    "clearServoErrors",
    "insertedAngleToAchieve",
    "setServoAngularPosition",
    "angle",
    "on_left_jog_released",
    "on_right_jog_released",
    "filterGeneralLog",
    "text",
    "filterServoLog",
    "updateGeneralLogContent",
    "updateServoLogContent",
    "updateActualServoData",
    "minas_control::MinasInput",
    "input",
    "readInputsUpdate",
    "operationOnOFFBehavior",
    "status",
    "setSensorData",
    "SensorData",
    "_data",
    "configTag_engenharia",
    "toggleTreeEditability"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10MainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  260,    2, 0x06,    1 /* Public */,
       4,    0,  263,    2, 0x06,    3 /* Public */,
       5,    0,  264,    2, 0x06,    4 /* Public */,
       6,    2,  265,    2, 0x06,    5 /* Public */,
       9,    0,  270,    2, 0x06,    8 /* Public */,
      10,    0,  271,    2, 0x06,    9 /* Public */,
      11,    3,  272,    2, 0x06,   10 /* Public */,
      14,    0,  279,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    1,  280,    2, 0x08,   15 /* Private */,
      18,    0,  283,    2, 0x08,   17 /* Private */,
      19,    0,  284,    2, 0x08,   18 /* Private */,
      20,    0,  285,    2, 0x08,   19 /* Private */,
      21,    0,  286,    2, 0x08,   20 /* Private */,
      22,    0,  287,    2, 0x08,   21 /* Private */,
      23,    0,  288,    2, 0x08,   22 /* Private */,
      24,    0,  289,    2, 0x08,   23 /* Private */,
      25,    0,  290,    2, 0x08,   24 /* Private */,
      26,    0,  291,    2, 0x08,   25 /* Private */,
      27,    0,  292,    2, 0x08,   26 /* Private */,
      28,    0,  293,    2, 0x08,   27 /* Private */,
      29,    0,  294,    2, 0x08,   28 /* Private */,
      30,    1,  295,    2, 0x08,   29 /* Private */,
      31,    1,  298,    2, 0x08,   31 /* Private */,
      33,    0,  301,    2, 0x08,   33 /* Private */,
      34,    0,  302,    2, 0x08,   34 /* Private */,
      35,    0,  303,    2, 0x08,   35 /* Private */,
      36,    0,  304,    2, 0x08,   36 /* Private */,
      37,    0,  305,    2, 0x08,   37 /* Private */,
      38,    2,  306,    2, 0x08,   38 /* Private */,
      40,    0,  311,    2, 0x08,   41 /* Private */,
      41,    0,  312,    2, 0x08,   42 /* Private */,
      42,    1,  313,    2, 0x08,   43 /* Private */,
      44,    1,  316,    2, 0x08,   45 /* Private */,
      45,    0,  319,    2, 0x08,   47 /* Private */,
      46,    0,  320,    2, 0x08,   48 /* Private */,
      47,    1,  321,    2, 0x08,   49 /* Private */,
      50,    0,  324,    2, 0x08,   51 /* Private */,
      51,    1,  325,    2, 0x08,   52 /* Private */,
      53,    1,  328,    2, 0x08,   54 /* Private */,
      56,    0,  331,    2, 0x08,   56 /* Private */,
      57,    1,  332,    2, 0x08,   57 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   12,    8,   13,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   39,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   43,
    QMetaType::Void, QMetaType::QString,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 48,   49,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   52,
    QMetaType::Void, 0x80000000 | 54,   55,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10MainWindowE.offsetsAndSizes,
    qt_meta_data_ZN10MainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10MainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'servoCheckBoxChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'initSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopOperationSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveServoToPositionSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'startHomingSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetErrorsSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'jogSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'readInputNowSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSensorDependencies'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SensorData *, std::false_type>,
        // method 'by_sensorSelected_action'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionVer_Sensores_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAdicionar_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAtualizar_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionRemover_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionLogout_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_login_menuBar_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionVer_usu_rios_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAdicionar_2_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAtualizar_2_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionRemover_2_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'by_animate_dial_button_action'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'servoCommunicationBox_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'servoState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'stopOperation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initializeServo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startHoming'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearServoErrors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'insertedAngleToAchieve'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setServoAngularPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'on_left_jog_released'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_right_jog_released'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'filterGeneralLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'filterServoLog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateGeneralLogContent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateServoLogContent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateActualServoData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<minas_control::MinasInput, std::false_type>,
        // method 'readInputsUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'operationOnOFFBehavior'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setSensorData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SensorData, std::false_type>,
        // method 'configTag_engenharia'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleTreeEditability'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->servoCheckBoxChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->initSignal(); break;
        case 2: _t->stopOperationSignal(); break;
        case 3: _t->moveServoToPositionSignal((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 4: _t->startHomingSignal(); break;
        case 5: _t->resetErrorsSignal(); break;
        case 6: _t->jogSignal((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 7: _t->readInputNowSignal(); break;
        case 8: _t->updateSensorDependencies((*reinterpret_cast< std::add_pointer_t<SensorData*>>(_a[1]))); break;
        case 9: _t->by_sensorSelected_action(); break;
        case 10: _t->on_actionVer_Sensores_triggered(); break;
        case 11: _t->on_actionAdicionar_triggered(); break;
        case 12: _t->on_actionAtualizar_triggered(); break;
        case 13: _t->on_actionRemover_triggered(); break;
        case 14: _t->on_actionLogout_triggered(); break;
        case 15: _t->on_login_menuBar_triggered(); break;
        case 16: _t->on_actionVer_usu_rios_triggered(); break;
        case 17: _t->on_actionAdicionar_2_triggered(); break;
        case 18: _t->on_actionAtualizar_2_triggered(); break;
        case 19: _t->on_actionRemover_2_triggered(); break;
        case 20: _t->by_animate_dial_button_action(); break;
        case 21: _t->servoCommunicationBox_stateChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->servoState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: _t->stopOperation(); break;
        case 24: _t->initializeServo(); break;
        case 25: _t->startHoming(); break;
        case 26: _t->clearServoErrors(); break;
        case 27: _t->insertedAngleToAchieve(); break;
        case 28: _t->setServoAngularPosition((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 29: _t->on_left_jog_released(); break;
        case 30: _t->on_right_jog_released(); break;
        case 31: _t->filterGeneralLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 32: _t->filterServoLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 33: _t->updateGeneralLogContent(); break;
        case 34: _t->updateServoLogContent(); break;
        case 35: _t->updateActualServoData((*reinterpret_cast< std::add_pointer_t<minas_control::MinasInput>>(_a[1]))); break;
        case 36: _t->readInputsUpdate(); break;
        case 37: _t->operationOnOFFBehavior((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 38: _t->setSensorData((*reinterpret_cast< std::add_pointer_t<SensorData>>(_a[1]))); break;
        case 39: _t->configTag_engenharia(); break;
        case 40: _t->toggleTreeEditability((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MainWindow::*)(bool );
            if (_q_method_type _q_method = &MainWindow::servoCheckBoxChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::initSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::stopOperationSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)(double , double );
            if (_q_method_type _q_method = &MainWindow::moveServoToPositionSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::startHomingSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::resetErrorsSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)(double , double , double );
            if (_q_method_type _q_method = &MainWindow::jogSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::readInputNowSignal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10MainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 41)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::servoCheckBoxChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::initSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::stopOperationSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::moveServoToPositionSignal(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::startHomingSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MainWindow::resetErrorsSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MainWindow::jogSignal(double _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::readInputNowSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
