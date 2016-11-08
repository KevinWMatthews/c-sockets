cmake_minimum_required (VERSION 3.5.1)
project (Socket)

if (DEFINED CPPUTEST_HOME)
    message(STATUS "CppUTest found from CMake variable: ${CPPUTEST_HOME}")
elseif (DEFINED ENV{CPPUTEST_HOME})
    message(STATUS "CppUTest from from environment variable: ${CPPUTEST_HOME}")
else ()
    message("CppUTest not found! Set variable CPPUTEST_HOME in CMake or PATH.")
    return()
endif()

enable_language(C)
enable_language(CXX)

SET (Socket_VERSION_MAJOR 0)
SET (Socket_VERSION_MINOR 1)

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory (products/ClientDemo)
add_subdirectory (products/ServerDemo)
add_subdirectory (products/TcpServer)
add_subdirectory (products/UdpClient)
add_subdirectory (products/UdpClient2)
add_subdirectory (products/UdpListener)
