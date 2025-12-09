enable_testing()

add_executable(ReportTests
    xmlwritertests.cpp)

target_link_libraries(ReportTests
    PUBLIC
    Report
    GTest::gtest_main)



include(GoogleTest)
gtest_discover_tests(ReportTests)
