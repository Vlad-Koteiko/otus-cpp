cmake_minimum_required(VERSION 4.2)

project(server_modules LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

find_library(SQLITE3_LIB sqlite3)
find_package(boost_system QUIET)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(server
    ./src/main.cpp
)

target_sources(server
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ./modules/bd.cppm
        ./modules/commander.cppm
)

target_link_libraries(server PRIVATE 
        ${Boost_LIBRARIES}
        ${SQLITE3_LIB}
     )

target_compile_features(server PUBLIC cxx_std_23)

add_executable(tests ./test_version.cpp)

target_sources(tests
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ./modules/bd.cppm
        ./modules/commander.cppm
)

target_link_libraries(tests PUBLIC 
        ${Boost_LIBRARIES}
        ${SQLITE3_LIB}
     )

target_compile_features(tests PUBLIC cxx_std_23)
