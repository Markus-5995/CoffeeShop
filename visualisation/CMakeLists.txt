add_library(Visualisation SHARED
    include/visualisation/visualiser.hpp
    visualiser.cpp
    shopgenerator.hpp shopgenerator.cpp
    shapefactory.hpp shapefactory.cpp
    mugshape.hpp mugshape.cpp)


target_link_libraries(Visualisation
    PUBLIC
    MessageQueue
    PRIVATE
    boost::boost
    sfml::sfml)

target_include_directories(Visualisation
    PUBLIC
    include)
