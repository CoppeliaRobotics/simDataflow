cmake_minimum_required(VERSION 2.8.11)
project(v_repExtDataflow)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(VREP REQUIRED)

include_directories(${VREP_ROOT}/programming/include)
include_directories(${CMAKE_BINARY_DIR}/generated)
include_directories(${CMAKE_SOURCE_DIR}/node-classes)
include_directories(${CMAKE_SOURCE_DIR}/data-classes)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/external/QDataflowCanvas)
include_directories(${Boost_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/generated/stubs.cpp ${CMAKE_BINARY_DIR}/generated/stubs.h ${CMAKE_BINARY_DIR}/generated/lua_calltips.cpp
    COMMAND python ${CMAKE_SOURCE_DIR}/external/v_repStubsGen/generate.py --xml-file ${CMAKE_SOURCE_DIR}/callbacks.xml --lua-file ${CMAKE_SOURCE_DIR}/simExtDataflow.lua --gen-all ${CMAKE_BINARY_DIR}/generated
    DEPENDS ${CMAKE_SOURCE_DIR}/callbacks.xml)

set(SOURCES
    v_repExtDataflow.cpp
    debug.cpp
    DFData.cpp
    DFModel.cpp
    DFNode.cpp
    DFWindow.cpp
    data-classes/DFBang.cpp
    data-classes/DFScalar.cpp
    data-classes/DFString.cpp
    data-classes/DFVector.cpp
    node-classes/DFMathBinaryOperator.cpp
    node-classes/DFObjectPos.cpp
    node-classes/DFPrint.cpp
    node-classes/DFTrigger.cpp
    node-classes/DFVectorMathBinaryOperator.cpp
    node-classes/DFVectorPack.cpp
    node-classes/DFVectorUnpack.cpp
    external/QDataflowCanvas/qdataflowcanvas.cpp
    external/QDataflowCanvas/qdataflowmodel.cpp
    external/v_repPlusPlus/Plugin.cpp
)
add_library(v_repExtDataflow SHARED ${SOURCES} ${CMAKE_BINARY_DIR}/generated/stubs.cpp "${VREP_ROOT}/programming/common/v_repLib.cpp")

if(WIN32)
    set(SHLWAPI shlwapi)
else()
    set(SHLWAPI)
endif()

target_link_libraries(v_repExtDataflow ${Boost_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport ${SHLWAPI})
