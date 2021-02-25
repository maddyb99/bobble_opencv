# cmake_minimum_required(VERSION <specify CMake version here>)
project(bobble_opencv)

set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.10.2)

message(STATUS ${CMAKE_ANDROID_ARCH_ABI})

if (ANDROID_ABI)
#    FILE(GLOB OPENCV " ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_*.a")
#    message(STATUS ${OPENCV})
    set(CMAKE_CXX_FLAGS "-Wall -lm -lz -Wextra")

    include_directories(include)
    include_directories(${OpenCV_DIR}/jni/include)

    add_library(webp SHARED IMPORTED)
    add_library(webpmux SHARED IMPORTED)
    add_library(webpdemux SHARED IMPORTED)
    add_library( lib_opencv SHARED IMPORTED )

    set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)
    set_target_properties(webp PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libwebp.so)
    set_target_properties(webpmux PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libwebpmux.so)
    set_target_properties(webpdemux PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libwebpdemux.so)
    add_library(bobble_opencv SHARED main_android.cpp webp_manip.cpp)

    find_library( # Sets the name of the path variable.
            log-lib

            # Specifies the name of the NDK library that
            # you want CMake to locate.
            log )

    find_package (Threads)

#    add_executable(bobble_opencv main.cpp head_api.cpp include/head_api.hpp webp_manip.cpp include/webp_manip.hpp)
    target_link_libraries(bobble_opencv ${log-lib} lib_opencv webp webpmux webpdemux Threads::Threads)

else()
    find_package( OpenCV REQUIRED )
    include_directories( ${OpenCV_INCLUDE_DIRS} )

    find_package(CURL REQUIRED)
    include_directories(${CURL_INCLUDE_DIRS})

    find_package (Threads)

    find_library(WEBP NAMES webp libwebp)

    find_library(WEBPMUX NAMES webpmux libwebpmux)

    #find_library(IMAGEIO NAMES imageioutil)
    #find_library(IMAGEIO NAMES imageio)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP jsoncpp)
    link_libraries(${JSONCPP_LIBRARIES})

    #find_package(WEBP REQUIRED)

    add_executable(bobble_opencv main.cpp head_api.cpp include/head_api.h webp_manip.cpp include/webp_manip.h)
    message(STATUS ${IMAGEIO})
    target_link_libraries(bobble_opencv ${OpenCV_LIBS} CURL::libcurl Threads::Threads ${JSONCPP_LIBRARIES} ${WEBP} ${WEBPMUX} /usr/lib/libwebpdemux.so)
    #target_link_libraries(bobble_opencv ${OpenCV_LIBS})
endif()
