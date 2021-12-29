QT       += core gui svg

qtHaveModule(opengl): QT += opengl

#requires(qtConfig(filedialog))

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lang/lexer.cpp \
    lang/parser.cpp \
    lang/token.cpp \
    lang/tree_node.cpp \
    main.cpp \
    ui/code-editor.cpp \
    ui/main-window.cpp \
    ui/svg-view.cpp \
    ui/tiny-highlighter.cpp \
    ui/token-highlighter.cpp

HEADERS += \
    lang/lexer.h \
    lang/parser.h \
    lang/tiny.h \
    lang/token.h \
    lang/tree_node.h \
    ui/code-editor.h \
    ui/main-window.h \
    ui/render-thread.h \
    ui/svg-view.h \
    ui/tiny-highlighter.h \
    ui/token-highlighter.h

RESOURCES = application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
