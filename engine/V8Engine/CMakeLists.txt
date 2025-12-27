
if (V8_SUPPORT)
    add_library(V8Engine STATIC
    coffeeshopmodule.hpp
    coffeeshopmodule.cpp
    simulation.hpp 
    simulation.cpp
    v8wrappers.hpp)

    target_link_libraries(V8Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8ppLab)
    
    target_compile_definitions(V8Engine
        PRIVATE
        DEFAULT_JS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../jscripts")
    
    target_compile_definitions(Engine PRIVATE V8_SUPPORT)
else()
    add_library(V8Engine INTERFACE)
endif()