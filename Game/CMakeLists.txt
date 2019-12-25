cmake_minimum_required(VERSION 3.0.0)
project(Game_Lib VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include)
aux_source_directory(./src SRC_LIST)
add_library(game STATIC ${SRC_LIST})
# add_library(game SHARED ${SRC_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
