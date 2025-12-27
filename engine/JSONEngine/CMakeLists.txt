add_library(JSONEngine STATIC
	jsonsim.hpp
	jsonsim.cpp)

target_link_libraries(JSONEngine
	PUBLIC
	Concepts
	MessageQueue
	PRIVATE
	nlohmann_json::nlohmann_json)
						