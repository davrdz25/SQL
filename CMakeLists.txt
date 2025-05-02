cmake_minimum_required(VERSION 3.16)
project(GraphQLServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

option(DISABLE_AUTO "No use auto" ON)

# Detect platform
if(WIN32)
    message(STATUS "Platform: Windows")
elseif(APPLE)
    message(STATUS "Platform: macOS")
elseif(UNIX)
    message(STATUS "Platform: Linux")
endif()

# Boost
#find_package(Boost REQUIRED)

# ODBC
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

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTKMM gtkmm-4.0 REQUIRED)

# cppgraphqlgen
#find_package(graphqlservice REQUIRED)

#target_include_directories(Server PRIVATE ${GTKMM_INCLUDE_DIRS})
#target_link_directories(Server PRIVATE ${GTKMM_LIBRARY_DIRS})
#target_link_libraries(Server PRIVATE ${GTKMM_LIBRARIES})

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories
    ${CMAKE_CURRENT_SOURCE_DIR}/Services
    ${CMAKE_CURRENT_SOURCE_DIR}/Models
)

# Source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Database/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Repositories/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Services/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
# Executable
add_executable(Server ${SRC_FILES})

# Link libraries
target_link_libraries(Server
    #${Boost_LIBRARIES}
    ${ODBC_LIBRARY}
    #graphqlservice
)


# macOS: link iconv if needed
if(APPLE)
    target_link_libraries(Server iconv)
endif()
