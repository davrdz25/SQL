cmake_minimum_required(VERSION 3.16)
project(Core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

if(WIN32)
    message(STATUS "Platform: Windows")
elseif(APPLE)
    message(STATUS "Platform: macOS")
elseif(UNIX)
    message(STATUS "Platform: Linux")
endif()

if(WIN32)
    find_library(ODBC_LIBRARY odbc32)
elseif(APPLE)
    find_library(ODBC_LIBRARY odbc)
elseif(UNIX)
    find_library(ODBC_LIBRARY odbc)
endif()

if(NOT ODBC_LIBRARY)
    message(FATAL_ERROR "ODBC library not found")
endif()

set(LIBCORE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories
    ${CMAKE_CURRENT_SOURCE_DIR}/Services
    ${CMAKE_CURRENT_SOURCE_DIR}/Models
)

include_directories(${LIBCORE_INCLUDE_DIRS})

file(GLOB_RECURSE LIBCORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Database/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Services/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.cpp
)

file(GLOB_RECURSE LIBCORE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Database/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Services/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.hpp
)

add_library(Core STATIC ${LIBCORE_SOURCES} ${LIBCORE_HEADERS})

target_link_libraries(Core ${ODBC_LIBRARY})
if(APPLE)
    target_link_libraries(Core iconv)
endif()

target_include_directories(Core PUBLIC ${LIBCORE_INCLUDE_DIRS})

set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    PUBLIC_HEADER "${LIBCORE_HEADERS}"
)

foreach(header IN LISTS LIBCORE_HEADERS)
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${header})
    get_filename_component(rel_dir ${rel_path} DIRECTORY)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/${rel_dir})
    configure_file(${header} ${CMAKE_BINARY_DIR}/include/${rel_path} COPYONLY)
endforeach()

