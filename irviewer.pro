QT       += core gui network printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

INCLUDEPATH += $$_PRO_FILE_PWD_\include \
    $$_PRO_FILE_PWD_\include\opencv-4.1.1 \
#    $$OPENCV_DIR\include

LIBS += -L$$_PRO_FILE_PWD_/lib \
#    -L$$OPENCV_DIR\x64\vc16\lib \
    -lavcodec\
    -lavutil\
    -lavformat\
    -lswscale\
    -lswresample \
    -lCrashRpt1403 \
    -lCrashRptProbe1403 \

CONFIG(debug, debug|release) {
    LIBS += -lopencv_world411d
} else {
    LIBS += -lopencv_world411
}

}

unix {
CONFIG += c++11
}
SOURCES += \
    aboutdlg.cpp \
    analyzer.cpp \
    callout.cpp \
    controlpanel.cpp \
    crashhandler.cpp \
    glcanvas.cpp \
    imagebutton.cpp \
    ipctrl.cpp \
    ircapturer.cpp \
    irxloader.cpp \
    irxparser.cpp \
    irxsaver.cpp \
    main.cpp \
    mainwindow.cpp \
    optiondlg.cpp \
    photothread.cpp \
    subbgdlg.cpp \
    utils.cpp \
    videothread.cpp

HEADERS += \
    aboutdlg.h \
    analyzer.h \
    callout.h \
    controlpanel.h \
    crashhandler.h \
    glcanvas.h \
    global.h \
    imagebutton.h \
    ipctrl.h \
    ircapturer.h \
    irx.h \
    irxloader.h \
    irxparser.h \
    irxsaver.h \
    mainwindow.h \
    optiondlg.h \
    photothread.h \
    subbgdlg.h \
    version.rc \
    videothread.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    swir.qrc

RC_ICONS = ir.ico
RC_FILE += version.rc

FORMS += \
    controlpanel.ui \
    optiondlg.ui \
    subbgdlg.ui
