cmake_minimum_required(VERSION 3.15)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(V8_SUPPORT "This option defines whether or not V8 shall be used to Configure the Shop" OFF)

project(CoffeeShop CXX)


if (V8_SUPPORT)
    FetchContent_Declare(
            V8Lab
            GIT_REPOSITORY https://github.com/Markus-5995/V8pp-Lab.git
            GIT_TAG main)
    FetchContent_MakeAvailable(V8Lab)
endif()

find_package(SFML REQUIRED)
find_package(Boost REQUIRED)
find_package(libxml2 REQUIRED)
find_package(nlohmann_json REQUIRED)

add_subdirectory(concepts)
add_subdirectory(messagequeue)
add_subdirectory(engine)
add_subdirectory(visualisation)
add_subdirectory(report)
add_subdirectory(jscripts)
add_subdirectory(jsonrecipes)


add_executable(CoffeeShop
        src/main.cpp)

target_link_libraries(CoffeeShop
        PUBLIC
        Engine
        Visualisation
        Report)
