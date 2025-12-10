cmake_minimum_required(VERSION 3.15)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(CoffeeShop CXX)

FetchContent_Declare(
        V8Lab
        GIT_REPOSITORY https://github.com/Markus-5995/V8pp-Lab.git
        GIT_TAG main)
FetchContent_MakeAvailable(V8Lab)

find_package(SFML REQUIRED)
find_package(Boost REQUIRED)
find_package(libxml2 REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(concepts)
add_subdirectory(messagequeue)
add_subdirectory(engine)
add_subdirectory(visualisation)
add_subdirectory(report)
add_subdirectory(jscripts)


add_executable(CoffeeShop
        src/main.cpp)

target_link_libraries(CoffeeShop
        PUBLIC
        Engine
        Visualisation
        Report)
