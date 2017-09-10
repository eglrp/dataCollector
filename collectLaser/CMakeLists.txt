PROJECT(collectLaser)



add_executable(collectLaser collectLaser.cpp)
target_link_libraries(collectLaser
        PacketDriver
        PacketDecoder
        pcap
        common
        pthread
        )

add_executable(parsingLaserData parsingLaserData.cpp)
target_link_libraries(parsingLaserData
        )

INSTALL(TARGETS collectLaser
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )

FILE(INSTALL ../third_parties/VelodyneHDL/16db.xml
        DESTINATION .)