PROJECT(collectInertial)



add_executable(collectInertial collectInertial.cpp)
target_link_libraries(collectInertial serial common)

add_executable(IMU_initialization IMU_initialization.cpp)
target_link_libraries(IMU_initialization serial common)

add_executable(parsingIMU parsingIMU.cpp)
target_link_libraries(parsingIMU serial common)

INSTALL(TARGETS collectInertial IMU_initialization
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )