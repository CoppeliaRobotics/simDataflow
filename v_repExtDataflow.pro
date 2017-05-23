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

lua_xml.target = generated/lua.xml
lua_xml.output = generated/lua.xml
lua_xml.input = simExtDataflow.lua
lua_xml.commands = python \"$$PWD/external/v_repStubsGen/lua_to_xml.py\" simExtDataflow.lua generated/lua.xml
QMAKE_EXTRA_TARGETS += lua_xml
PRE_TARGETDEPS += generated/lua.xml

reference_xml.target = generated/reference.xml
reference_xml.output = generated/reference.xml
reference_xml.input = callbacks.xml generated/lua.xml
reference_xml.commands = python \"$$PWD/external/v_repStubsGen/merge_xml.py\" callbacks.xml generated/lua.xml generated/reference.xml
QMAKE_EXTRA_TARGETS += reference_xml
PRE_TARGETDEPS += generated/reference.xml

reference_html.target = generated/reference.html
reference_html.output = generated/reference.html
reference_html.input = generated/reference.xml
reference_html.commands = xsltproc --path \"$$PWD/\" -o generated/reference.html \"$$PWD/external/v_repStubsGen/xsl/reference.xsl\" generated/reference.xml
QMAKE_EXTRA_TARGETS += reference_html
PRE_TARGETDEPS += generated/reference.html

lua_calltips_cpp.target = generated/lua_calltips.cpp
lua_calltips_cpp.output = generated/lua_calltips.cpp
lua_calltips_cpp.input = simExtDataflow.lua \"$$PWD/external/v_repStubsGen/generate_lua_calltips.py\"
lua_calltips_cpp.commands = python \"$$PWD/external/v_repStubsGen/generate_lua_calltips.py\" Dataflow DF \"$$PWD/simExtDataflow.lua\" \"$$PWD/generated/lua_calltips.cpp\"
QMAKE_EXTRA_TARGETS += lua_calltips_cpp
PRE_TARGETDEPS += generated/lua_calltips.cpp

HEADERS += \
    DFNode.h \
    node-classes/DFMathBinaryOperator.h \
    node-classes/DFObjectPos.h \
    node-classes/DFPrint.h \
    node-classes/DFVectorMathBinaryOperator.h \
    node-classes/DFVectorPack.h \
    node-classes/DFVectorUnpack.h \
    DFData.h \
    data-classes/DFScalar.h \
    data-classes/DFString.h \
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
    node-classes/DFVectorMathBinaryOperator.cpp \
    node-classes/DFVectorPack.cpp \
    node-classes/DFVectorUnpack.cpp \
    DFData.cpp \
    data-classes/DFScalar.cpp \
    data-classes/DFString.cpp \
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


