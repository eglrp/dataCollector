PROJECT(collectInertial)



add_executable(collectInertial collectInertial.cpp)
target_link_libraries(collectInertial serial common)

add_executable(parsingIMU parsingIMU.cpp)
target_link_libraries(parsingIMU serial common)

INSTALL(TARGETS collectInertial
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )