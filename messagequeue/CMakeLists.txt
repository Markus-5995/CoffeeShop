add_library(MessageQueue SHARED
    include/messagequeue/producer.hpp
    include/messagequeue/consumer.hpp
    include/messagequeue/message.hpp
    include/messagequeue/messagebus.hpp
    consumer.cpp
    producer.cpp
    messagebus.cpp)


target_link_libraries(MessageQueue
    PUBLIC
    Concepts)

target_include_directories(MessageQueue
    PUBLIC
    include)


set_target_properties(MessageQueue PROPERTIES LINKER_LANGUAGE CXX)
