# cmake_minimum_required(VERSION <specify CMake version here>)
project(bobble_opencv)

set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.19)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS} )

find_package (Threads)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

#find_package(WEBP REQUIRED)

add_executable(bobble_opencv main.cpp head_api.cpp include/head_api.hpp webp_manip.cpp include/webp_manip.hpp)
message(STATUS ${CURL_LIBS})
target_link_libraries(bobble_opencv ${OpenCV_LIBS} CURL::libcurl Threads::Threads ${JSONCPP_LIBRARIES})
#target_link_libraries(bobble_opencv ${OpenCV_LIBS})