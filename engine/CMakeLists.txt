

add_library(Engine SHARED
    include/engine/coffeeshopmodule.hpp
    src/coffeeshopmodule.cpp
    src/simulation.hpp
    src/v8wrappers.hpp)


target_link_libraries(Engine
    PUBLIC
    V8ppLab
    Concepts
    MessageQueue
    boost::boost)

target_include_directories(Engine
    PUBLIC
    include)
