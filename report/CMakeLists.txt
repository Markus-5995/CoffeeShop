add_library(Report SHARED
    include/report/simulationreport.hpp
    include/report/reportexport.hpp
    simulationreport.cpp
    simulationparser.hpp simulationparser.cpp
    streamwriter.hpp)

target_link_libraries(Report PUBLIC
    MessageQueue
    PRIVATE
    LibXml2::LibXml2
    boost::boost)

target_include_directories(Report PUBLIC
    include)

target_compile_definitions(Report PRIVATE EXPORT_REPORT)

#add_subdirectory(tests)

add_executable(ReportApp
    main.cpp)

target_link_libraries(ReportApp
    Report)

