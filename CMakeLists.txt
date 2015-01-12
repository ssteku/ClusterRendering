cmake_minimum_required (VERSION 2.6)
PROJECT(ClusterRendering)

SET(CMAKE_CXX_COMPILER clang)
set(CMAKE_CXX_FLAGS "-g3 -Wall -DDEBUG -O3 -Wconversion -Wformat -std=c++11")

include_directories(./xsd/xsd/libxsd /usr/include/xsd)

file(GLOB_RECURSE ClusterRendering_SOURCES_CPP "src/*.cpp")
file(GLOB_RECURSE ClusterRendering_SOURCES_CXX "src/*.cxx")
file(GLOB_RECURSE ClusterRendering_HEADERS_H "src/*.h")
file(GLOB_RECURSE ClusterRendering_HEADERS_HPP "src/*.hpp")
file(GLOB_RECURSE ClusterRendering_HEADERS_HXX "src/*.hxx")

set ( headers_files ${headers_files} ${ClusterRendering_HEADERS_H} ${ClusterRendering_HEADERS_HPP} ${ClusterRendering_HEADERS_HXX} )
set (ClusterRendering_INCLUDE_DIRS "")
foreach (_headerFile ${headers_files})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ClusterRendering_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES ClusterRendering_INCLUDE_DIRS)
include_directories(${ClusterRendering_INCLUDE_DIRS})
add_executable (ClusterRenderingExe ${ClusterRendering_SOURCES_CPP} ${ClusterRendering_SOURCES_CXX})
link_directories(/usr/lib/x86_64-linux-gnu/)

target_link_libraries(ClusterRenderingExe -lstdc++ boost_filesystem -lboost_system png OgreMain xerces-c -lm)