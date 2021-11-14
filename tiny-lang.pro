QT       += core gui

#requires(qtConfig(filedialog))

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    lang/lexer.cc \
    ui/code-editor.cpp \
    ui/main-window.cpp \
    ui/tiny-highlighter.cpp

HEADERS += \
    lang/lexer.hh \
    lang/logger.hh\
    lang/magic_enum.hh\
    lang/tiny.hh\
    lang/token.hh\
    ui/code-editor.h \
    ui/main-window.h \
    ui/tiny-highlighter.h

RESOURCES = application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
