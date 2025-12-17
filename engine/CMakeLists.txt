add_library(Engine SHARED
    include/engine/engine.hpp
    include/engine/engineexport.hpp
    engine.cpp)

if (V8_SUPPORT)
    add_library(V8Engine STATIC
    V8Engine/coffeeshopmodule.hpp
    V8Engine/coffeeshopmodule.cpp
    V8Engine/simulation.hpp 
    V8Engine/simulation.cpp
    V8Engine/v8wrappers.hpp)

    target_link_libraries(V8Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8ppLab
    boost::boost)
    
    target_compile_definitions(V8Engine
        PRIVATE
        DEFAULT_JS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../jscripts")
    
    target_compile_definitions(Engine PRIVATE V8_SUPPORT)
else()
    add_library(V8Engine INTERFACE)
endif()

add_library(JSONEngine STATIC
    JSONEngine/simulation.hpp
    JSONEngine/simulation.cpp)

target_link_libraries(JSONEngine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    boost::boost)


target_link_libraries(Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8Engine
    JSONEngine
    boost::boost)

target_include_directories(Engine
    PUBLIC
    include)
    
target_compile_definitions(Engine PRIVATE EXPORT_ENGINE)

add_executable(EngineApp
    main.cpp)

target_link_libraries(EngineApp
    Engine)
