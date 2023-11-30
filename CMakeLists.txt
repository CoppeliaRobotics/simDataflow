cmake_minimum_required(VERSION 2.8.11)
project(simDataflow)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

if(NOT COPPELIASIM_INCLUDE_DIR)
    if(DEFINED ENV{COPPELIASIM_ROOT_DIR})
        set(COPPELIASIM_INCLUDE_DIR $ENV{COPPELIASIM_ROOT_DIR}/programming/include)
    else()
        set(COPPELIASIM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    endif()
endif()
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
    ${COPPELIASIM_INCLUDE_DIR}/cmake)
find_package(CoppeliaSim 4.5.1.7 REQUIRED)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/node-classes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data-classes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/QDataflowCanvas)
add_definitions(-DQT_COMPIL)

coppeliasim_generate_stubs(${CMAKE_CURRENT_BINARY_DIR}/generated XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/sourceCode/callbacks.xml LUA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lua/simDataflow.lua)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sourceCode/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(SOURCES
    sourceCode/plugin.cpp
    sourceCode/DFData.cpp
    sourceCode/DFModel.cpp
    sourceCode/DFNode.cpp
    sourceCode/DFWindow.cpp
    sourceCode/data-classes/DFBang.cpp
    sourceCode/data-classes/DFScalar.cpp
    sourceCode/data-classes/DFString.cpp
    sourceCode/data-classes/DFVector.cpp
    sourceCode/node-classes/DFMathBinaryOperator.cpp
    sourceCode/node-classes/DFObjectPos.cpp
    sourceCode/node-classes/DFPrint.cpp
    sourceCode/node-classes/DFTrigger.cpp
    sourceCode/node-classes/DFVectorMathBinaryOperator.cpp
    sourceCode/node-classes/DFVectorPack.cpp
    sourceCode/node-classes/DFVectorUnpack.cpp
    external/QDataflowCanvas/qdataflowcanvas.cpp
    external/QDataflowCanvas/qdataflowmodel.cpp
)
coppeliasim_add_plugin(simDataflow SOURCES ${SOURCES})
target_link_libraries(simDataflow Boost::boost Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
