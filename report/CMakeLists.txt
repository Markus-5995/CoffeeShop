add_library(Report SHARED
    include/report/simulationreport.hpp
    simulationreport.cpp
    simulationparser.hpp simulationparser.cpp
    streamwriter.hpp)

target_link_libraries(Report PUBLIC
    LibXml2::LibXml2
    MessageQueue
    PRIVATE
    boost::boost)

target_include_directories(Report PUBLIC
    include)

add_subdirectory(tests)
