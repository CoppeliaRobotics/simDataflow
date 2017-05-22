QT += core gui widgets

TARGET = v_repExtDataflow
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared debug_and_release
INCLUDEPATH += "../include"
INCLUDEPATH += "external/QDataflowCanvas"
INCLUDEPATH += "external"
INCLUDEPATH += "generated"
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

generated.target = $$PWD/generated
generated.output = $$PWD/generated
generated.commands = $(MKDIR) $$PWD/generated
QMAKE_EXTRA_TARGETS += generated
PRE_TARGETDEPS += $$PWD/generated

stubs_h.target = generated/stubs.h
stubs_h.output = generated/stubs.h
stubs_h.input = callbacks.xml $$PWD/generated
stubs_h.commands = python \"$$PWD/external/v_repStubsGen/main.py\" -H generated/stubs.h callbacks.xml
QMAKE_EXTRA_TARGETS += stubs_h
PRE_TARGETDEPS += generated/stubs.h

stubs_cpp.target = generated/stubs.cpp
stubs_cpp.output = generated/stubs.cpp
stubs_cpp.input = callbacks.xml $$PWD/generated
stubs_cpp.commands = python \"$$PWD/external/v_repStubsGen/main.py\" -C generated/stubs.cpp callbacks.xml
QMAKE_EXTRA_TARGETS += stubs_cpp
PRE_TARGETDEPS += generated/stubs.cpp

reference_html.target = generated/reference.html
reference_html.output = generated/reference.html
reference_html.input = callbacks.xml $$PWD/generated
reference_html.commands = xsltproc --path \"$$PWD/\" -o \"$$PWD/generated/reference.html\" \"$$PWD/external/v_repStubsGen/xsl/reference.xsl\" callbacks.xml
QMAKE_EXTRA_TARGETS += reference_html
PRE_TARGETDEPS += generated/reference.html

HEADERS += \
    DFNode.h \
    node-classes/DFMathBinaryOperator.h \
    node-classes/DFObjectPos.h \
    node-classes/DFPrint.h \
    DFData.h \
    data-classes/DFVector.h \
    DFNodeFactory.h \
    DFEventsListener.h \
    UIProxy.h \
    DFWindow.h \
    plugin.h \
    debug.h \
    v_repExtDataflow.h \
    external/v_repPlusPlus/Plugin.h \
    external/QDataflowCanvas/qdataflowcanvas.h \
    external/QDataflowCanvas/qdataflowmodel.h \
    ../include/v_repLib.h \
    generated/stubs.h

SOURCES += \
    DFNode.cpp \
    node-classes/DFMathBinaryOperator.cpp \
    node-classes/DFObjectPos.cpp \
    node-classes/DFPrint.cpp \
    DFData.cpp \
    data-classes/DFVector.cpp \
    DFNodeFactory.cpp \
    DFEventsListener.cpp \
    UIProxy.cpp \
    DFWindow.cpp \
    debug.cpp \
    v_repExtDataflow.cpp \
    external/v_repPlusPlus/Plugin.cpp \
    external/QDataflowCanvas/qdataflowcanvas.cpp \
    external/QDataflowCanvas/qdataflowmodel.cpp \
    ../common/v_repLib.cpp \
    generated/stubs.cpp


