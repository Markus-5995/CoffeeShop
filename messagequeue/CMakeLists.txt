add_library(MessageQueue SHARED
    include/messagequeue/producer.hpp
    include/messagequeue/consumer.hpp
    include/messagequeue/message.hpp
    include/messagequeue/messagebus.hpp
    include/messagequeue/messagequeueexport.hpp
    consumer.cpp
    producer.cpp
    messagebus.cpp
    driverconcept.hpp
    tcpipdriver.hpp tcpipdriver.cpp
    tcpserver.cpp tcpserver.hpp
    tcpclient.hpp tcpclient.cpp
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

    
target_compile_definitions(MessageQueue PRIVATE EXPORT_MSGQ)
