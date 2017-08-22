cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
project(dataCollector)


# third_parties
# nmea
add_subdirectory(third_parties/NmeaTode)
include_directories(third_parties/NmeaTode/include)
# serial
add_subdirectory(third_parties/serial)
include_directories(third_parties/serial/include)



add_subdirectory(timeSychron)
add_subdirectory(collectInertial)

