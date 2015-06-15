cmake_minimum_required(VERSION 2.6)
project(DASHSource)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
include(ExternalProject)

set(curl_VERSION "7.42.1")
set(tinyxml2_VERSION "3.0.0")
set(curl_URI http://curl.haxx.se/download/curl-${curl_VERSION}.tar.gz)
set(curl_local ${CMAKE_CURRENT_SOURCE_DIR}/tarballs/curl-${curl_VERSION}.tar.gz)
set(tiny_URL https://github.com/leethomason/tinyxml2/archive/${tinyxml2_VERSION}.tar.gz)
set(tiny_local ${CMAKE_CURRENT_SOURCE_DIR}/tarballs/${tinyxml2_VERSION}.tar.gz)

#source dirs
set(curl_DIR ${EXTERNALS_DIR}/curl${curl_VERSION})
set(tinyxml_DIR ${EXTERNALS_DIR}/tinyxml${tinyxml2_VERSION})
#binary dirs
set(curl_bin ${CMAKE_BINARY_DIR}/externals/libcurl${curl_VERSION})
set(tiny_bin ${CMAKE_BINARY_DIR}/externals/tinyxml${tinyxml2_VERSION})

#curl libs and headers
set(curl_lib ${curl_bin} ${curl_bin}/Debug ${curl_bin}/Release)
set(curl_include ${curl_DIR}/include/curl)

#tinyxml2 libs and headers
set(tinyxml_lib ${tiny_bin} ${tiny_bin}/Debug ${tiny_bin}/Release)
set(tinyxml_include ${tinyxml_DIR})

if(NOT EXISTS "${curl_local}")
    file(DOWNLOAD ${curl_URI} ${curl_local})
endif()
if(NOT EXISTS "${tiny_local}")
    file(DOWNLOAD ${tiny_URL} ${tiny_local})
endif()

#include_directories
#include_directories(${tiny_inc} ${curl_inc})
#link_directories(${tiny_lib} ${curl_lib})

##########################################
ExternalProject_Add(
	curl-build
	URL ${curl_local}
	SOURCE_DIR "${curl_DIR}"
	DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tarballs"
	CMAKE_ARGS -DBUILD_CURL_TESTS:BOOL=OFF
				-DBUILD_CURL_EXE:BOOL=OFF
				-DCURL_DISABLE_LDAP:BOOL=ON
				-DCMAKE_USE_OPENSSL:BOOL=OFF
				-DCURL_ZLIB:BOOL=ON
				-DCMAKE_USE_LIBSSH2:BOOL=OFF
				-DCURL_STATIC_LIB:BOOL=ON
	BINARY_DIR ${curl_bin}
	INSTALL_COMMAND ""
	TEST_COMMAND ""
)


ExternalProject_Add(
    tinyxml2-build
    BINARY_DIR ${tiny_bin}
    DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tarballs"
    URL ${tiny_local}
    SOURCE_DIR ${tinyxml_DIR}
    CMAKE_ARGS ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

add_subdirectory(app)
add_subdirectory(lib)
