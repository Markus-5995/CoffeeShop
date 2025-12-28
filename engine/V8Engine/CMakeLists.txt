
if (V8_SUPPORT)
    add_library(V8Engine STATIC
    coffeeshopmodule.hpp
    coffeeshopmodule.cpp
    v8engine.hpp 
    v8engine.cpp
    v8wrappers.hpp
    simulation.hpp
    simulation.cpp)

    target_link_libraries(V8Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8ppLab)
    target_compile_definitions(V8Engine PRIVATE V8_SUPPORT)
else()
    add_library(V8Engine STATIC
    simulation.hpp
    simulation.cpp)

    target_link_libraries(V8Engine
    PUBLIC
    Concepts
    MessageQueue)
endif()
    
target_compile_definitions(V8Engine
    PUBLIC
    DEFAULT_JS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../jscripts")
    