include_directories (${PROJECT_SOURCE_DIR}/inc)
include_directories (${PROJECT_SOURCE_DIR}/src_platform)

SET (source_code
    ${PROJECT_SOURCE_DIR}/src/Socket.c
    ${PROJECT_SOURCE_DIR}/src_platform/SocketSystemLayer.c)
add_executable (TcpServer TcpServer.c ${source_code})
target_link_libraries (TcpServer pthread)
