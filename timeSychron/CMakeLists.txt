project(timeSychron)

find_package(Threads REQUIRED)

set(SRC
        gpioOprations.hpp)

add_executable(timeSychron timeSychron.cpp ${SRC})
target_link_libraries(timeSychron ${CMAKE_THREAD_LIBS_INIT})

INSTALL(TARGETS timeSychron
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )