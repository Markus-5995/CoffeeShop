add_library(Concepts SHARED
    include/concepts/coffeemachine.hpp
    include/concepts/entity.hpp
    include/concepts/mug.hpp
    include/concepts/world.hpp
    include/concepts/simulationcontext.hpp
    include/concepts/coffeemachine.cpp
    include/concepts/mug.cpp
    include/concepts/simulationcontext.cpp
)

target_include_directories(Concepts
    PUBLIC
    include)

target_link_libraries(Concepts
    PUBLIC
    Boost::boost)

set_target_properties(Concepts PROPERTIES LINKER_LANGUAGE CXX)
