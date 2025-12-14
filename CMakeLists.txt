cmake_minimum_required(VERSION 3.28)

project(server_modules LANGUAGES CXX)

find_library(SQLITE3_LIB sqlite3)
find_package(boost_system QUIET)

add_executable(main
    ./src/main.cpp
)

target_sources(main
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ./modules/bd.cppm
        ./modules/commander.cppm
)

target_link_libraries(main PRIVATE 
        ${Boost_LIBRARIES}
        ${SQLITE3_LIB}
     )

target_compile_features(main PUBLIC cxx_std_23)
