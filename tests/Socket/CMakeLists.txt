include_directories (${PROJECT_SOURCE_DIR}/inc)
include_directories (${PROJECT_SOURCE_DIR}/src_platform)
include_directories (${CPPUTEST_HOME}/include)

# Find CppUTest libraries
find_library (lib_cpputest libCppUTest.a)
find_library (lib_cpputest_ext libCppUTestExt.a)
add_library (lib_cpputest STATIC IMPORTED)
add_library (lib_cpputest_ext STATIC IMPORTED)

# Define platform-independent source code that will be tested.
SET (source
    ${PROJECT_SOURCE_DIR}/src/Socket.c)

# Archive it to allow link-time substitution.
# If we decide to deploy this library, this statement will
# probably move to the source folder.
add_library (Socket ${source})

# Define mocked platform-dependent code.
SET (mock_src_platform
    ${PROJECT_SOURCE_DIR}/mocks/Socket/SocketSystemLayer.cpp)

# Define test code.
SET (test_source
    Test_Socket.cpp
    Test_SocketClient.cpp
    Test_SocketServer.cpp
    TestHelpers.cpp)

add_executable (SocketTests AllTests.cpp ${test_source} ${mock_src_platform} ${source})
target_link_libraries (SocketTests libCppUTest.a libCppUTestExt.a Socket)
