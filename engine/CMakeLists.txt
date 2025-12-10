

add_library(Engine SHARED
    include/engine/engine.hpp
    engine.cpp
    coffeeshopmodule.hpp
    coffeeshopmodule.cpp
    simulation.hpp
    v8wrappers.hpp)

target_link_libraries(Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8ppLab
    boost::boost)

target_include_directories(Engine
    PUBLIC
    include)

target_compile_definitions(Engine
    PRIVATE
    DEFAULT_JS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../jscripts")


add_executable(EngineApp
    main.cpp)

target_link_libraries(EngineApp
    Engine)
