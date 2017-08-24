PROJECT(collectImages)

find_library(POINTGREY_LIB NAMES libflycapture.so.2 flycapture)
find_path(POINTGREY_INCLUDE_DIR NAMES flycapture/FlyCapture2.h)

if(NOT POINTGREY_LIB OR NOT POINTGREY_INCLUDE_DIR)
    message(FATAL_ERROR "can not find libflycapture")
else()
    message(STATUS "libflycapture found in system library path")
    message(STATUS "libflycapture library: ${POINTGREY_LIB}")
    message(STATUS "libflycapture include: ${POINTGREY_INCLUDE_DIR}")
    include_directories(${POINTGREY_INCLUDE_DIR})
endif()

add_executable(collectImages collectImages.cpp)
target_link_libraries(collectImages ${POINTGREY_LIB})


set(flyTest "ON" )

if(flyTest STREQUAL "ON")
    add_executable(flyTest TEST/flyTest.cpp)
    target_link_libraries(flyTest ${POINTGREY_LIB})
endif()