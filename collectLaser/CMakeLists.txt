PROJECT(collectLaser)



add_executable(collectLaser collectLaser.cpp)
target_link_libraries(collectLaser
        PacketDriver
        PacketDecoder
        pcap
        )

INSTALL(TARGETS collectLaser
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )