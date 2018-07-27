cmake_minimum_required(VERSION 2.8.11)
project(v_repExtDataflow)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(Boost REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoppeliaRobotics-modules)
find_package(VREP 3.5.0 REQUIRED)

include_directories(${VREP_INCLUDE})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/node-classes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data-classes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/QDataflowCanvas)
include_directories(${Boost_INCLUDE_DIRS})

vrep_generate_stubs(${CMAKE_CURRENT_BINARY_DIR}/generated XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.xml LUA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/simExtDataflow.lua)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(SOURCES
    plugin.cpp
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
add_library(v_repExtDataflow SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/generated/stubs.cpp "${VREP_EXPORTED_SOURCES}")
target_link_libraries(v_repExtDataflow ${Boost_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport ${VREP_LIBRARIES})
