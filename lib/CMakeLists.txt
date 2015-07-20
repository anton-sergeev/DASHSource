cmake_minimum_required(VERSION 2.8)

set(PROJECT DASHSourceLib)
project(${PROJECT})

add_definitions(-g)

set(SOURCES
  src/CurlReceiver.cpp
  src/DASHSource.cpp
  src/MPDManager.cpp
  src/RatioType.cpp
  src/tegger.cpp
  src/URLList.cpp
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${curl_include}
	${tinyxml_include}
)

# link_directories(
# 	${tinyxml_lib}
# 	${curl_lib}
# )

add_library(${PROJECT} STATIC ${SOURCES})
# set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME dashsrclib)

add_dependencies(${PROJECT} tinyxml2-build curl-build)

if(UNIX)
	target_link_libraries(${PROJECT} pthread)
endif()
