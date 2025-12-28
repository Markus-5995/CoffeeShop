add_subdirectory(V8Engine)
add_subdirectory(CppEngine)
add_subdirectory(JSONEngine)


add_library(Engine SHARED
    include/engine/engine.hpp
    include/engine/engineexport.hpp
    engine.cpp "V8Engine/simulation.cpp")

target_link_libraries(Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8Engine
    CppEngine
    JSONEngine)
    

target_include_directories(Engine
    PUBLIC
    include)
    
target_compile_definitions(Engine PRIVATE EXPORT_ENGINE)

add_executable(EngineApp
    main.cpp)

target_link_libraries(EngineApp
    Engine)
