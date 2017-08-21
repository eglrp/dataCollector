cmake_minimum_required(VERSION 2.8.3)
project(serial)

## Sources
set(serial_SRCS
    src/serial.cc
    include/serial/serial.h
    include/serial/v8stdint.h
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# If unix
list(APPEND serial_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/unix.cc)
list(APPEND serial_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/list_ports/list_ports_linux.cc)


## Add serial library
add_library(${PROJECT_NAME} ${serial_SRCS})

#TEST
set(serial_test "ON")
if(serial_test STREQUAL "ON")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_executable(serial_demo TEST/serial_example.cc)
    target_link_libraries(serial_demo serial)
endif()
