project(timeSychronArdu)
add_compile_options(-fPIC)
find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5SerialPort_INCLUDES_DIRS})

set(CMAKE_AUTOMOC ON) ## important
set(SRC
        arduCommunicator.cpp
        arduCommunicator.h)

add_executable(timeSychronArdu timeSychronArdu.cpp ${SRC})
target_link_libraries(timeSychronArdu serial common ${Qt5Core_LIBRARIES} ${Qt5SerialPort_LIBRARIES})


INSTALL(TARGETS timeSychronArdu
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )