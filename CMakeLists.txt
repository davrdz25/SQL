cmake_minimum_required(VERSION 3.10)
project(GraphQL_API_Cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# Definir directorios de inclusión
include_directories(include)

# Buscar Boost
#find_package(Boost REQUIRED COMPONENTS system filesystem)
#include_directories(${Boost_INCLUDE_DIRS})

# Buscar ODBC
find_package(ODBC REQUIRED)

# Buscar cppgraphqlgen
#find_package(cppgraphqlgen REQUIRED)

# Archivos fuente
set(SOURCES
    main.cpp
    include/SQL/SQL.cpp
    include/Dataset/Dataset.cpp
)

# Crear el ejecutable
add_executable(server ${SOURCES})

# Enlazar librerías
#target_link_libraries(server odbc ${Boost_LIBRARIES} cppgraphqlgen)
target_link_libraries(server odbc)

