/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created: Wed 12. Dec 15:33:07 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../glwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GlWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      37,   28,    9,    9, 0x08,
      57,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GlWidget[] = {
    "GlWidget\0\0showColorDialog()\0newColor\0"
    "changeColor(QColor)\0updateCoordinates()\0"
};

void GlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GlWidget *_t = static_cast<GlWidget *>(_o);
        switch (_id) {
        case 0: _t->showColorDialog(); break;
        case 1: _t->changeColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->updateCoordinates(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GlWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GlWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GlWidget,
      qt_meta_data_GlWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GlWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GlWidget))
        return static_cast<void*>(const_cast< GlWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
