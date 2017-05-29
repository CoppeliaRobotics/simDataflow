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

gen_all.target = generated/stubs.h
gen_all.output = generated/stubs.h
gen_all.input = callbacks.xml simExtCustomUI.lua
gen_all.commands = python \"$$PWD/external/v_repStubsGen/generate.py\" --xml-file \"$$PWD/callbacks.xml\" --lua-file \"$$PWD/simExtDataflow.lua\" --gen-all \"$$PWD/generated/\"
QMAKE_EXTRA_TARGETS += gen_all
PRE_TARGETDEPS += generated/stubs.h

HEADERS += \
    DFNode.h \
    node-classes/DFMathBinaryOperator.h \
    node-classes/DFObjectPos.h \
    node-classes/DFPrint.h \
    node-classes/DFTrigger.h \
    node-classes/DFVectorMathBinaryOperator.h \
    node-classes/DFVectorPack.h \
    node-classes/DFVectorUnpack.h \
    DFData.h \
    data-classes/DFBang.h \
    data-classes/DFScalar.h \
    data-classes/DFString.h \
    data-classes/DFVector.h \
    DFModel.h \
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
    node-classes/DFTrigger.cpp \
    node-classes/DFVectorMathBinaryOperator.cpp \
    node-classes/DFVectorPack.cpp \
    node-classes/DFVectorUnpack.cpp \
    DFData.cpp \
    data-classes/DFBang.cpp \
    data-classes/DFScalar.cpp \
    data-classes/DFString.cpp \
    data-classes/DFVector.cpp \
    DFModel.cpp \
    DFWindow.cpp \
    debug.cpp \
    v_repExtDataflow.cpp \
    external/v_repPlusPlus/Plugin.cpp \
    external/QDataflowCanvas/qdataflowcanvas.cpp \
    external/QDataflowCanvas/qdataflowmodel.cpp \
    ../common/v_repLib.cpp \
    generated/stubs.cpp


