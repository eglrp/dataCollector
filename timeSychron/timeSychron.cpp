//
// Created by jianping on 17-8-20.
//

#include "gpioOprations.hpp"
//#include "linux/time.h"
#include<thread>
#include <pthread.h>
#include<chrono>
#include <functional>



int main()
{
    /// register all the gpio
    //we use I2S0_SDO1/GPIO3_D6 for output check
    //3x32+30 = 126
    const int gpio_check = 126;
    GPIO::gpio_export(gpio_check);
    GPIO::gpio_set_dir(gpio_check,GPIO::Direction::OUT);

    //we use I2S0_SDO2/GPIO3_D5 for input the GPS PPS
    //3x32+29 = 125
    const int goio_pps = 125;
    GPIO::gpio_export(goio_pps);
    GPIO::gpio_set_dir(goio_pps,GPIO::Direction::IN);


    // use buzzer to make some noise for telling us that the machine is working now, quite stupid
    std::function<void(void)> checkFunction = [&](void)->void{
        while (1)
        {
            GPIO::gpio_set_value(gpio_check,GPIO::Pin::HIGH);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            GPIO::gpio_set_value(gpio_check,GPIO::Pin::LOW);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    };

    std::thread checkThread(checkFunction);


    checkThread.join();

    GPIO::gpio_unexport(gpio_check);
    GPIO::gpio_unexport(goio_pps);

    return 0;

}