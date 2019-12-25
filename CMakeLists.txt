cmake_minimum_required(VERSION 3.0.0)
project(RepeatedPlay VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./Game/include)
add_subdirectory(./Game)
aux_source_directory(. SRC_LIST)
add_executable(repeatedPlay ${SRC_LIST})
target_link_libraries(repeatedPlay game)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
