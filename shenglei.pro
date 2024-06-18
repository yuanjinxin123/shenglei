QT       += core gui serialport sql concurrent network
greaterThan(QT_MAJOR_VERSION, 5):QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#TARGET=
#TEMPLATE=app
CONFIG += c++17
MOC_DIR=obj
OBJECTS_DIR = obj
UI_DIR=obj
DESTDIR = bin

#DEFINES += DXJG_SHENGXIONG

if(contains(DEFINES,DXJG_SHENGXIONG)) {
    TARGET=shengxiong
    RC_FILE = logo.rc
} else {
    TARGET=shenglei
    RC_FILE = logo_t.rc
}

win32-msvc* {
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QS_LOG_LINE_NUMBERS
include(thrid/QsLog/QsLog.pri)
SOURCES += \
    GraphicsView.cpp \
    HighestWidgetTab.cpp \
    MacoustoWidgetTab.cpp \
    WorkSpaceWidget.cpp \
    config.cpp \
    dlg_act.cpp \
    iport.cpp \
    jlcompressex.cpp \
    loadingdialog.cpp \
    log.cpp \
    mFristPage.cpp \
    macousto_pod.cpp \
    macousto_pso.cpp \
    macoustobase.cpp \
    macoustoparam.cpp \
    maichongwdt.cpp \
    main.cpp \
    mainwindow.cpp \
    mconnectdlg.cpp \
    mcorrectiondlg.cpp \
    mdatecheckdlg.cpp \
    melectric.cpp \
    meletdlgadmin.cpp \
    mexportdatedlg.cpp \
    mfucwdt.cpp \
    mhighestwdt.cpp \
    mlogindlg.cpp \
    motherwdt.cpp \
    mportmanager.cpp \
    mpowerchage.cpp \
    mpowerinfowdt.cpp \
    mpsobasewdt.cpp \
    mpsowdt.cpp \
    mseedparamwdt.cpp \
    msempwdt.cpp \
    mserial.cpp \
    msettingdlg.cpp \
    mtoolbtn.cpp \
    mwarminfo.cpp \
    mwarnwdt.cpp \
    mwidget.cpp \
    mytitlebar.cpp \
    portcfg.cpp \
    spinbox.cpp \
    sql.cpp \
    titlebar.cpp \
    xiaohao.cpp

HEADERS += \
    GraphicsView.h \
    HighestWidgetTab.h \
    MacoustoWidgetTab.h \
    WorkSpaceWidget.h \
    config.h \
    dlg_act.h \
    iport \
    jlcompressex.h \
    loadingdialog.h \
    log.h \
    mFristPage.h \
    macousto_pod.h \
    macousto_pso.h \
    macoustobase.h \
    macoustoparam.h \
    maichongwdt.h \
    mainwindow.h \
    mconnectdlg.h \
    mcorrectiondlg.h \
    mdatecheckdlg.h \
    melectric.h \
    meletdlgadmin.h \
    mexportdatedlg.h \
    mfucwdt.h \
    mhighestwdt.h \
    mlogindlg.h \
    motherwdt.h \
    mportmanager.h \
    mpowerchage.h \
    mpowerinfowdt.h \
    mpsobasewdt.h \
    mpsowdt.h \
    mseedparamwdt.h \
    msempwdt.h \
    mserial.h \
    msettingdlg.h \
    mtoolbtn.h \
    mwarminfo.h \
    mwarnmap.h \
    mwarnwdt.h \
    mwidget.h \
    mytitlebar.h \
    order.h \
    portcfg.h \
    spinbox.h \
    sql.h \
    titlebar.h \
    xiaohao.h

FORMS += \
    dlg_act.ui \
    loadingdialog.ui \
    mFristPage.ui \
    macousto_pod.ui \
    macousto_pso.ui \
    macoustobase.ui \
    macoustoparam.ui \
    maichongwdt.ui \
    mainwindow.ui \
    mconnectdlg.ui \
    mcorrectiondlg.ui \
    mdatecheckdlg.ui \
    melectric.ui \
    meletdlgadmin.ui \
    mexportdatedlg.ui \
    mfucwdt.ui \
    mhighestwdt.ui \
    mlogindlg.ui \
    motherwdt.ui \
    mpowerchage.ui \
    mpowerinfowdt.ui \
    mpsobasewdt.ui \
    mpsowdt.ui \
    mseedparamwdt.ui \
    msempwdt.ui \
    msettingdlg.ui \
    mwarnwdt.ui \
    portcfg.ui \
    xiaohao.ui

TRANSLATIONS += \
    shenglei_zh_CN.ts
#CONFIG += lrelease
#CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    pic.qrc \
    q.qrc



win32: LIBS += -L$$PWD/thrid/zlib/lib/ -lquazip

INCLUDEPATH += $$PWD/thrid/zlib/include
DEPENDPATH += $$PWD/thrid/zlib/include
