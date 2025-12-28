add_library(JSONEngine STATIC
	jsonsim.hpp
	jsonsim.cpp)

target_link_libraries(JSONEngine
	PUBLIC
	Concepts
	MessageQueue
	PRIVATE
	nlohmann_json::nlohmann_json)
						
	
target_compile_definitions(JSONEngine
    PUBLIC
    DEFAULT_JSON_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../jsonrecipes/CoffeeShopConfig.json")