add_library(Visualisation SHARED
    include/visualisation/visualiser.hpp
    include/visualisation/visualisationexport.hpp
    visualiser.cpp
    shopgenerator.hpp shopgenerator.cpp
    shapefactory.hpp shapefactory.cpp
    mugshape.hpp mugshape.cpp)


target_link_libraries(Visualisation
    PUBLIC
    MessageQueue
    PRIVATE
    sfml::sfml)

target_include_directories(Visualisation
    PUBLIC
    include)

target_compile_definitions(Visualisation PRIVATE EXPORT_VIS)

add_executable(VisualisationApp
    main.cpp)

target_link_libraries(VisualisationApp
    Visualisation)

