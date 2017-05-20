QT += core gui widgets

TARGET = v_repExtDataflow
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared debug_and_release
INCLUDEPATH += "../include"
INCLUDEPATH += "data-classes"
INCLUDEPATH += "node-classes"

*-msvc* {
	QMAKE_CXXFLAGS += -O2
	QMAKE_CXXFLAGS += -W3
}
*-g++*|*clang* {
        CONFIG(debug, debug|release){
            QMAKE_CXXFLAGS += -g
            QMAKE_CFLAGS += -g
        } else {
            QMAKE_CXXFLAGS += -O3
            QMAKE_CFLAGS += -O3
        }
        QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter
        QMAKE_CXXFLAGS_WARN_OFF = -Wall -Wno-unused-parameter
}

win32 {
    DEFINES += WIN_VREP
    DEFINES += NOMINMAX
    DEFINES += strcasecmp=_stricmp
    INCLUDEPATH += "c:/local/boost_1_62_0"
}

macx {
    QMAKE_MAC_SDK = macosx10.12
    DEFINES += MAC_VREP
    INCLUDEPATH += "/usr/local/Cellar/boost/1.63.0/include/"
}

unix:!macx {
    DEFINES += LIN_VREP
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

stubs_h.target = stubs.h
stubs_h.output = stubs.h
stubs_h.input = callbacks.xml
stubs_h.commands = python \"$$PWD/external/v_repStubsGen/main.py\" -H stubs.h callbacks.xml
QMAKE_EXTRA_TARGETS += stubs_h
PRE_TARGETDEPS += stubs.h

stubs_cpp.target = stubs.cpp
stubs_cpp.output = stubs.cpp
stubs_cpp.input = callbacks.xml
stubs_cpp.commands = python \"$$PWD/external/v_repStubsGen/main.py\" -C stubs.cpp callbacks.xml
QMAKE_EXTRA_TARGETS += stubs_cpp
PRE_TARGETDEPS += stubs.cpp

reference_html.target = reference.html
reference_html.output = reference.html
reference_html.input = callbacks.xml
reference_html.commands = xsltproc --path \"$$PWD/\" -o reference.html \"$$PWD/external/v_repStubsGen/xsl/reference.xsl\" callbacks.xml
QMAKE_EXTRA_TARGETS += reference_html
PRE_TARGETDEPS += reference.html

HEADERS += \
    DFNode.h \
    node-classes/DFObjectPos.h \
    node-classes/DFPrint.h \
    DFData.h \
    data-classes/DFVector.h \
    nodeFactory.h \
    plugin.h \
    v_repExtDataflow.h \
    ../include/v_repLib.h \
    stubs.h

SOURCES += \
    DFNode.cpp \
    node-classes/DFObjectPos.cpp \
    node-classes/DFPrint.cpp \
    DFData.cpp \
    data-classes/DFVector.cpp \
    nodeFactory.cpp \
    v_repExtDataflow.cpp \
    ../common/v_repLib.cpp \
    stubs.cpp


