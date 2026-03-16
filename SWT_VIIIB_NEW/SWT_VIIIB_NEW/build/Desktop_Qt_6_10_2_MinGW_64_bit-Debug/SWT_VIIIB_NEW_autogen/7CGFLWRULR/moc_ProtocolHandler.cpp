/****************************************************************************
** Meta object code from reading C++ file 'ProtocolHandler.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../comm_dev/protocol/ProtocolHandler.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProtocolHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
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
struct qt_meta_tag_ZN15ProtocolHandlerE_t {};
} // unnamed namespace

template <> constexpr inline auto ProtocolHandler::qt_create_metaobjectdata<qt_meta_tag_ZN15ProtocolHandlerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProtocolHandler",
        "ioInitFinished",
        "",
        "success",
        "voltageRegAckReceived",
        "voltageRegResultReceived",
        "errorCode",
        "realVoltage",
        "logMessage",
        "msg",
        "onDataReceived",
        "rawData"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ioInitFinished'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'voltageRegAckReceived'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'voltageRegResultReceived'
        QtMocHelpers::SignalData<void(quint8, float)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UChar, 6 }, { QMetaType::Float, 7 },
        }}),
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'onDataReceived'
        QtMocHelpers::SlotData<void(const QByteArray &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProtocolHandler, qt_meta_tag_ZN15ProtocolHandlerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProtocolHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProtocolHandlerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProtocolHandlerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ProtocolHandlerE_t>.metaTypes,
    nullptr
} };

void ProtocolHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProtocolHandler *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ioInitFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->voltageRegAckReceived((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->voltageRegResultReceived((*reinterpret_cast<std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        case 3: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onDataReceived((*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProtocolHandler::*)(bool )>(_a, &ProtocolHandler::ioInitFinished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProtocolHandler::*)(bool )>(_a, &ProtocolHandler::voltageRegAckReceived, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProtocolHandler::*)(quint8 , float )>(_a, &ProtocolHandler::voltageRegResultReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProtocolHandler::*)(const QString & )>(_a, &ProtocolHandler::logMessage, 3))
            return;
    }
}

const QMetaObject *ProtocolHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProtocolHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProtocolHandlerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProtocolHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ProtocolHandler::ioInitFinished(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ProtocolHandler::voltageRegAckReceived(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ProtocolHandler::voltageRegResultReceived(quint8 _t1, float _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void ProtocolHandler::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
