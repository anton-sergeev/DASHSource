cmake_minimum_required(VERSION 2.6)
project(DASHSource)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
include(ExternalProject)

set(curl_VERSION "7.42.1")
set(curl_URI http://curl.haxx.se/download/curl-${curl_VERSION}.tar.gz)
set(curl_DIR ${EXTERNALS_DIR}/curl${curl_VERSION})
set(tinyxml_DIR ${EXTERNALS_DIR}/tinyxml)

#curl libs and headers
set(curl_lib ${CMAKE_BINARY_DIR}/externals/libcurl${curl_VERSION})
set(curl_include ${curl_DIR}/include/curl)

#tinyxml2 libs and headers
set(tinyxml_lib ${CMAKE_BINARY_DIR}/externals/tinyxml)
set(tinyxml_include ${tinyxml_DIR})


#include_directories

ExternalProject_Add(
	curl-build
	URL ${curl_URI}
	SOURCE_DIR "${curl_DIR}"
	DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tarballs/curl/${curl_VERSION}"
	#CMAKE_ARGS "-DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DCURL_DISABLE_LDAP=ON -DCMAKE_USE_OPENSSL=OFF -DCURL_ZLIB
	CMAKE_ARGS -DBUILD_CURL_TESTS:BOOL=OFF
				-DBUILD_CURL_EXE:BOOL=OFF
				-DCURL_DISABLE_LDAP:BOOL=ON
				-DCMAKE_USE_OPENSSL:BOOL=OFF
				-DCURL_ZLIB:BOOL=ON
				-DCMAKE_USE_LIBSSH2:BOOL=OFF
	#-DCURL_STATICLIB:BOOL=ON
	#CMAKE_CACHE_ARGS "-DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DCURL_DISABLE_LDAP=ON -DCMAKE_USE_OPENSSL=OFF -DCUR
	#CMAKE_CACHE_ARGS -DBUILD_CURL_TESTS:BOOL=OFF -DBUILD_CURL_EXE:BOOL=OFF -DCURL_DISABLE_LDAP:BOOL=ON -DCMAKE_USE_OPEOPENSSL=OFF
	#CMAKE_CACHE_ARGS BUILD_CURL_TESTS:BOOL=OFF BUILD_CURL_EXE:BOOL=OFF CURL_DISABLE_LDAP:BOOL=ON CMAKE_USE_OPENSSL:BOO
# 	CONFIGURE_COMMAND ""
	#BUILD_COMMAND ""
	BINARY_DIR ${CMAKE_BINARY_DIR}/externals/libcurl${curl_VERSION}
	INSTALL_COMMAND ""
	TEST_COMMAND ""
)


ExternalProject_Add(
    tinyxml2-build
    BINARY_DIR ${CMAKE_BINARY_DIR}/externals/tinyxml
    DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tarballs/tinyxml2"
    URL https://github.com/leethomason/tinyxml2/archive/master.zip
    SOURCE_DIR ${tinyxml_DIR}
    CMAKE_ARGS ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

add_subdirectory(lib)
