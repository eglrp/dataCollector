PROJECT(NmeaTode)

include_directories(./include)

aux_source_directory(./src NmeaTode_SRC)

add_library(NmeaTode ${NmeaTode_SRC})


#test
set(NmeaTodeTest "OFF")

if(NmeaTodeTest STREQUAL "ON")
    add_executable(nmea_test TEST/nmea_test.cpp)
    target_link_libraries(nmea_test NmeaTode)
endif()

