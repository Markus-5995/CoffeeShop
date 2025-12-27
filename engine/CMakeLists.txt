add_subdirectory(V8Engine)
add_subdirectory(CppEngine)
add_subdirectory(JSONEngine)


add_library(Engine SHARED
    include/engine/engine.hpp
    include/engine/engineexport.hpp
    engine.cpp)

target_link_libraries(Engine
    PUBLIC
    Concepts
    MessageQueue
    PRIVATE
    V8Engine
    CppEngine
    JSONEngine)
    
target_compile_definitions(Engine
    PRIVATE
    DEFAULT_JSON_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../jsonrecipes/CoffeeShopConfig.json")

target_include_directories(Engine
    PUBLIC
    include)
    
target_compile_definitions(Engine PRIVATE EXPORT_ENGINE)

add_executable(EngineApp
    main.cpp)

target_link_libraries(EngineApp
    Engine)
