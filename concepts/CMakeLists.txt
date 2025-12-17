add_library(Concepts SHARED
    include/concepts/coffeemachine.hpp
    include/concepts/entity.hpp
    include/concepts/mug.hpp
    include/concepts/world.hpp
    include/concepts/coffeemachine.cpp
    include/concepts/mug.cpp
    include/concepts/world.cpp
    include/concepts/actortypes.hpp
    include/concepts/conceptexport.hpp
    include/concepts/simulationcontext.hpp 
    include/concepts/simulationcontext.cpp
)

target_include_directories(Concepts
    PUBLIC
    include)

target_link_libraries(Concepts
    PRIVATE
    boost::boost)

target_compile_definitions(Concepts PRIVATE EXPORT_CONCEPTS)
