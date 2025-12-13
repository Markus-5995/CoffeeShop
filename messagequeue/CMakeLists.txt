add_library(MessageQueue SHARED
    include/messagequeue/producer.hpp
    include/messagequeue/consumer.hpp
    include/messagequeue/message.hpp
    include/messagequeue/messagebus.hpp
    consumer.cpp
    producer.cpp
    messagebus.cpp
    driverconcept.hpp
    tcpipdriver.hpp tcpipdriver.cpp
    tcpimpl.hpp tcpimpl.cpp
    simplequeuedriver.hpp simplequeuedriver.cpp
    multiconsumerqueuedriver.hpp multiconsumerqueuedriver.cpp)


target_link_libraries(MessageQueue
    PUBLIC
    Concepts
    PRIVATE
    boost::boost)

target_include_directories(MessageQueue
    PUBLIC
    include)


set_target_properties(MessageQueue PROPERTIES LINKER_LANGUAGE CXX)
