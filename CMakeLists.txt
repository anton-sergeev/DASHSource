cmake_minimum_required(VERSION 2.6)
project(DASHSource)

set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
include(ExternalProject)

set(curl_VERSION "7.42.1")
set(curl_URI http://curl.haxx.se/download/curl-${curl_VERSION}.tar.gz)
set(curl_DIR ${EXTERNALS_DIR}/curl${curl_VERSION})
set(tinyxml_DIR ${EXTERNALS_DIR}/tinyxml)
include_directories(${curl_DIR}include/curl/)

ExternalProject_Add(
    libcurl
    BINARY_DIR ${CMAKE_BINARY_DIR}/externals/curl${curl_VERSION}
    URL ${curl_URI}
    SOURCE_DIR ${curl_DIR}
    CMAKE_ARGS "-DBUILD_CURL_EXE:BOOL=OFF -DBUILD_CURL_TESTS:BOOL=OFF -DCURL_STATICLIB:BOOL=ON -DENABLE_ARES:BOOL=OFF -DENABLE_THREADED_RESOLVER:BOOL=OFF -DENABLE_CURLDEBUG:BOOL=OFF -DENABLE_DEBUG:BOOL=OFF"
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
ExternalProject_Add(
    tinyxml2
    BINARY_DIR ${CMAKE_BINARY_DIR}/externals/tinyxml
    URL https://github.com/leethomason/tinyxml2/archive/master.zip
    SOURCE_DIR ${tinyxml_DIR}
    CMAKE_ARGS ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)