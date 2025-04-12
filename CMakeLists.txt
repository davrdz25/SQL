cmake_minimum_required(VERSION 3.16)
project(GraphQLServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
find_package(Boost REQUIRED)

# ODBC
if(WIN32)
    find_library(ODBC_LIBRARY odbc32)
elseif(APPLE)
    find_library(ODBC_LIBRARY iodbc)
elseif(UNIX)
    find_library(ODBC_LIBRARY odbc)
endif()

if(NOT ODBC_LIBRARY)
    message(FATAL_ERROR "ODBC library not found")
endif()

# cppgraphqlgen
#find_package(graphqlservice REQUIRED)

# Include directories
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/include/DataTable/*.cpp
    ${CMAKE_SOURCE_DIR}/include/SQL/*.cpp
#    ${CMAKE_SOURCE_DIR}/include/Models/**/*.cpp
    ${CMAKE_SOURCE_DIR}/include/GlobalEnvironment/*.cpp
#    ${CMAKE_SOURCE_DIR}/include/GraphQL/QueryResolver/*.cpp
)

# Executable
add_executable(GraphQLServer ${SOURCES})

# Link libraries
target_link_libraries(GraphQLServer
    ${Boost_LIBRARIES}
    ${ODBC_LIBRARY}
    graphqlservice
)

# macOS: link iconv if needed
if(APPLE)
    target_link_libraries(GraphQLServer iconv)
endif()
