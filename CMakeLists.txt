cmake_minimum_required (VERSION 2.6)
PROJECT(ClusterRendering)

SET(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_FLAGS "-g3 -Wall -DDEBUG -O2 ")

include_directories(/usr/include/mysql ./xsd/xsd/libxsd /etc/alternatives/mpi /usr/include/xsd)

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

target_link_libraries(ClusterRenderingExe boost_mpi-mt boost_serialization-mt mysqlpp mysqlclient xerces-c mpi mpi_cxx boost_thread-mt png) 