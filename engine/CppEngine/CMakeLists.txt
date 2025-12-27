
add_library(CppEngine STATIC
    simulation.hpp
    simulation.cpp)

target_link_libraries(CppEngine
    PUBLIC
    Concepts
    MessageQueue)
