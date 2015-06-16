cmake_minimum_required(VERSION 2.8)

set(PROJECT DASHSourceTest)
project(${PROJECT})

set(SOURCES
  main.cpp
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

include_directories(
	${CMAKE_SOURCE_DIR}/lib/include
)

link_directories(
	${curl_bin}/lib
	${tiny_bin}
)

if(MSVC)
	set(LIBCURL libcurl_imp)
else()
	set(LIBCURL curl)
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT}
	DASHSourceLib
	${LIBCURL}
	tinyxml2
)
set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME dashsrc_test)
