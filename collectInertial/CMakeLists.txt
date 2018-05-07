PROJECT(collectInertial)
set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-fPIC")

find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5SerialPort_INCLUDES_DIRS})

set(CMAKE_AUTOMOC ON) ## important


add_executable(collectInertial collectInertial.cpp)
target_link_libraries(collectInertial serial common ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES})

add_executable(IMU_initialization IMU_initialization.cpp)
target_link_libraries(IMU_initialization serial common ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES})

add_executable(parsingIMU parsingIMU.cpp)
target_link_libraries(parsingIMU serial common ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES})

INSTALL(TARGETS collectInertial IMU_initialization
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )