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

    add_library(opencv_core STATIC IMPORTED)
    add_library(opencv_imgproc STATIC IMPORTED)
    add_library(opencv_video STATIC IMPORTED)
    add_library(opencv_videoio STATIC IMPORTED)
    add_library(opencv_highgui STATIC IMPORTED)
    add_library(opencv_imgcodecs STATIC IMPORTED)
    add_library(opencv_ade STATIC IMPORTED)

    add_library(tiff STATIC IMPORTED)
    add_library(libpng STATIC IMPORTED)
    add_library(libjpeg STATIC IMPORTED)
    add_library(libjasper STATIC IMPORTED)
    add_library(libprotobuf STATIC IMPORTED)
    add_library(libquirc STATIC IMPORTED)

    add_library(webp SHARED IMPORTED)
    add_library(webpmux SHARED IMPORTED)

    find_library( # Sets the name of the path variable.
            log-lib

            # Specifies the name of the NDK library that
            # you want CMake to locate.
            log )

    find_package (Threads)

    set_target_properties(opencv_core PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_core.a)
    set_target_properties(opencv_imgproc PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_imgproc.a)
    set_target_properties(opencv_video PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_video.a)
    set_target_properties(opencv_videoio PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_videoio.a)
    set_target_properties(opencv_highgui PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_highgui.a)
    set_target_properties(opencv_imgcodecs PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libopencv_imgcodecs.a)
    set_target_properties(opencv_ade PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libade.a)


    set_target_properties(tiff PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/liblibtiff.a)
    set_target_properties(libpng PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/liblibpng.a)
    set_target_properties(libjpeg PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/liblibjpeg-turbo.a)
    set_target_properties(libprotobuf PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/liblibprotobuf.a)
    set_target_properties(libquirc PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/libquirc.a)
    set_target_properties(libjasper PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/3rdparty/liblibjasper.a)

    set_target_properties(webp PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libwebp.so)
    set_target_properties(webpmux PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/${ANDROID_ABI}/libwebpmux.so)
    add_library(bobble_opencv SHARED webp_manip.cpp)
#    add_executable(bobble_opencv main.cpp head_api.cpp include/head_api.hpp webp_manip.cpp include/webp_manip.hpp)
    target_link_libraries(bobble_opencv ${log-lib} opencv_core opencv_imgproc opencv_video opencv_highgui opencv_videoio opencv_imgcodecs opencv_ade tiff libjpeg libpng libprotobuf libquirc libjasper webp webpmux Threads::Threads)

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

    add_executable(bobble_opencv main.cpp head_api.cpp include/head_api.hpp webp_manip.cpp include/webp_manip.hpp)
    message(STATUS ${IMAGEIO})
    target_link_libraries(bobble_opencv ${OpenCV_LIBS} CURL::libcurl Threads::Threads ${JSONCPP_LIBRARIES} ${WEBP} ${WEBPMUX} /usr/lib/libwebpdemux.so)
    #target_link_libraries(bobble_opencv ${OpenCV_LIBS})
endif()
