/****************************************************************************
** Meta object code from reading C++ file 'imagedraw.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ImageDraw/imagedraw.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagedraw.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageDraw_t {
    QByteArrayData data[43];
    char stringdata0[887];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageDraw_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageDraw_t qt_meta_stringdata_ImageDraw = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ImageDraw"
QT_MOC_LITERAL(1, 10, 11), // "startWorker"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 17), // "refreshZoomWindow"
QT_MOC_LITERAL(4, 41, 18), // "pointClickReceived"
QT_MOC_LITERAL(5, 60, 25), // "on_SetCoordButton_clicked"
QT_MOC_LITERAL(6, 86, 15), // "on_Exit_clicked"
QT_MOC_LITERAL(7, 102, 22), // "on_SelectImage_clicked"
QT_MOC_LITERAL(8, 125, 22), // "on_StartButton_clicked"
QT_MOC_LITERAL(9, 148, 17), // "onDrawingComplete"
QT_MOC_LITERAL(10, 166, 19), // "onDrawingTerminated"
QT_MOC_LITERAL(11, 186, 28), // "on_FittoFrameCB_stateChanged"
QT_MOC_LITERAL(12, 215, 4), // "arg1"
QT_MOC_LITERAL(13, 220, 31), // "on_algodrop_currentIndexChanged"
QT_MOC_LITERAL(14, 252, 5), // "index"
QT_MOC_LITERAL(15, 258, 34), // "on_artboardrop_currentIndexCh..."
QT_MOC_LITERAL(16, 293, 31), // "on_cropAndCentercb_stateChanged"
QT_MOC_LITERAL(17, 325, 35), // "on_CropComboBox_currentIndexC..."
QT_MOC_LITERAL(18, 361, 28), // "on_scaletoartCB_stateChanged"
QT_MOC_LITERAL(19, 390, 25), // "on_BgFillEdit_textChanged"
QT_MOC_LITERAL(20, 416, 29), // "on_blockboundsCB_stateChanged"
QT_MOC_LITERAL(21, 446, 15), // "emitPauseSignal"
QT_MOC_LITERAL(22, 462, 8), // "drawNeon"
QT_MOC_LITERAL(23, 471, 2), // "c1"
QT_MOC_LITERAL(24, 474, 2), // "c2"
QT_MOC_LITERAL(25, 477, 20), // "on_color1But_clicked"
QT_MOC_LITERAL(26, 498, 23), // "on_coordConfirm_clicked"
QT_MOC_LITERAL(27, 522, 28), // "on_changeToCoordsBut_clicked"
QT_MOC_LITERAL(28, 551, 24), // "on_prepareButton_clicked"
QT_MOC_LITERAL(29, 576, 28), // "on_stayTopCheck_stateChanged"
QT_MOC_LITERAL(30, 605, 21), // "on_endDrawing_clicked"
QT_MOC_LITERAL(31, 627, 20), // "on_color2But_clicked"
QT_MOC_LITERAL(32, 648, 22), // "on_clearButton_clicked"
QT_MOC_LITERAL(33, 671, 20), // "on_SaveState_clicked"
QT_MOC_LITERAL(34, 692, 13), // "loadSaveState"
QT_MOC_LITERAL(35, 706, 25), // "on_clicksEdit_textChanged"
QT_MOC_LITERAL(36, 732, 12), // "updateColors"
QT_MOC_LITERAL(37, 745, 23), // "on_globEdit_textChanged"
QT_MOC_LITERAL(38, 769, 30), // "on_uisleepinterval_textChanged"
QT_MOC_LITERAL(39, 800, 19), // "on_minimize_clicked"
QT_MOC_LITERAL(40, 820, 21), // "on_helpbutton_clicked"
QT_MOC_LITERAL(41, 842, 23), // "on_letsgobutton_clicked"
QT_MOC_LITERAL(42, 866, 20) // "incrementProgressBar"

    },
    "ImageDraw\0startWorker\0\0refreshZoomWindow\0"
    "pointClickReceived\0on_SetCoordButton_clicked\0"
    "on_Exit_clicked\0on_SelectImage_clicked\0"
    "on_StartButton_clicked\0onDrawingComplete\0"
    "onDrawingTerminated\0on_FittoFrameCB_stateChanged\0"
    "arg1\0on_algodrop_currentIndexChanged\0"
    "index\0on_artboardrop_currentIndexChanged\0"
    "on_cropAndCentercb_stateChanged\0"
    "on_CropComboBox_currentIndexChanged\0"
    "on_scaletoartCB_stateChanged\0"
    "on_BgFillEdit_textChanged\0"
    "on_blockboundsCB_stateChanged\0"
    "emitPauseSignal\0drawNeon\0c1\0c2\0"
    "on_color1But_clicked\0on_coordConfirm_clicked\0"
    "on_changeToCoordsBut_clicked\0"
    "on_prepareButton_clicked\0"
    "on_stayTopCheck_stateChanged\0"
    "on_endDrawing_clicked\0on_color2But_clicked\0"
    "on_clearButton_clicked\0on_SaveState_clicked\0"
    "loadSaveState\0on_clicksEdit_textChanged\0"
    "updateColors\0on_globEdit_textChanged\0"
    "on_uisleepinterval_textChanged\0"
    "on_minimize_clicked\0on_helpbutton_clicked\0"
    "on_letsgobutton_clicked\0incrementProgressBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageDraw[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  199,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  200,    2, 0x0a /* Public */,
       4,    0,  201,    2, 0x08 /* Private */,
       5,    0,  202,    2, 0x08 /* Private */,
       6,    0,  203,    2, 0x08 /* Private */,
       7,    0,  204,    2, 0x08 /* Private */,
       8,    0,  205,    2, 0x08 /* Private */,
       9,    0,  206,    2, 0x08 /* Private */,
      10,    0,  207,    2, 0x08 /* Private */,
      11,    1,  208,    2, 0x08 /* Private */,
      13,    1,  211,    2, 0x08 /* Private */,
      15,    1,  214,    2, 0x08 /* Private */,
      16,    1,  217,    2, 0x08 /* Private */,
      17,    1,  220,    2, 0x08 /* Private */,
      18,    1,  223,    2, 0x08 /* Private */,
      19,    1,  226,    2, 0x08 /* Private */,
      20,    0,  229,    2, 0x08 /* Private */,
      21,    0,  230,    2, 0x08 /* Private */,
      22,    2,  231,    2, 0x08 /* Private */,
      25,    0,  236,    2, 0x08 /* Private */,
      26,    0,  237,    2, 0x08 /* Private */,
      27,    0,  238,    2, 0x08 /* Private */,
      28,    0,  239,    2, 0x08 /* Private */,
      29,    1,  240,    2, 0x08 /* Private */,
      30,    0,  243,    2, 0x08 /* Private */,
      31,    0,  244,    2, 0x08 /* Private */,
      32,    0,  245,    2, 0x08 /* Private */,
      33,    0,  246,    2, 0x08 /* Private */,
      34,    0,  247,    2, 0x08 /* Private */,
      35,    0,  248,    2, 0x08 /* Private */,
      36,    0,  249,    2, 0x08 /* Private */,
      37,    0,  250,    2, 0x08 /* Private */,
      38,    0,  251,    2, 0x08 /* Private */,
      39,    0,  252,    2, 0x08 /* Private */,
      40,    0,  253,    2, 0x08 /* Private */,
      41,    0,  254,    2, 0x08 /* Private */,
      42,    0,  255,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor, QMetaType::QColor,   23,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
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
    QMetaType::Void,

       0        // eod
};

void ImageDraw::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageDraw *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startWorker(); break;
        case 1: _t->refreshZoomWindow(); break;
        case 2: _t->pointClickReceived(); break;
        case 3: _t->on_SetCoordButton_clicked(); break;
        case 4: _t->on_Exit_clicked(); break;
        case 5: _t->on_SelectImage_clicked(); break;
        case 6: _t->on_StartButton_clicked(); break;
        case 7: _t->onDrawingComplete(); break;
        case 8: _t->onDrawingTerminated(); break;
        case 9: _t->on_FittoFrameCB_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_algodrop_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_artboardrop_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_cropAndCentercb_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_CropComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_scaletoartCB_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_BgFillEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->on_blockboundsCB_stateChanged(); break;
        case 17: _t->emitPauseSignal(); break;
        case 18: _t->drawNeon((*reinterpret_cast< QColor(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 19: _t->on_color1But_clicked(); break;
        case 20: _t->on_coordConfirm_clicked(); break;
        case 21: _t->on_changeToCoordsBut_clicked(); break;
        case 22: _t->on_prepareButton_clicked(); break;
        case 23: _t->on_stayTopCheck_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->on_endDrawing_clicked(); break;
        case 25: _t->on_color2But_clicked(); break;
        case 26: _t->on_clearButton_clicked(); break;
        case 27: _t->on_SaveState_clicked(); break;
        case 28: _t->loadSaveState(); break;
        case 29: _t->on_clicksEdit_textChanged(); break;
        case 30: _t->updateColors(); break;
        case 31: _t->on_globEdit_textChanged(); break;
        case 32: _t->on_uisleepinterval_textChanged(); break;
        case 33: _t->on_minimize_clicked(); break;
        case 34: _t->on_helpbutton_clicked(); break;
        case 35: _t->on_letsgobutton_clicked(); break;
        case 36: _t->incrementProgressBar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageDraw::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageDraw::startWorker)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageDraw::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ImageDraw.data,
    qt_meta_data_ImageDraw,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageDraw::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageDraw::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageDraw.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ImageDraw::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 37;
    }
    return _id;
}

// SIGNAL 0
void ImageDraw::startWorker()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
