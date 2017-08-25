PROJECT(common)


set(commom_scr
        common.hpp
        common.cpp)

add_library(common ${commom_scr})

set(commontest "ON")
if(commontest STREQUAL "ON")
    add_executable(common_test TEST/commontest.cpp)
    target_link_libraries(common_test common)
endif()