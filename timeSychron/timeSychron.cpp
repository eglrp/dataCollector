//
// Created by jianping on 17-8-20.
//

#include "gpioOprations.hpp"
#include "forgeNMEA.hpp"
#include "sys/time.h"
#include<thread>
#include <mutex>
#include<chrono>
#include <functional>
#include <iostream>




int main()
{
//    /// register all the gpio
//    //we use I2S0_SDO1/GPIO3_D6 for output check
//    //3x32+30 = 126
//    const int gpio_check = 126;
//    GPIO::gpio_export(gpio_check);
//    GPIO::gpio_set_dir(gpio_check,GPIO::Direction::OUT);
//
//    //we use I2S0_SDO2/GPIO3_D5 for output the GPS PPS
//    //3x32+29 = 125
//    const int goio_pps = 125;
//    GPIO::gpio_export(goio_pps);
//    GPIO::gpio_set_dir(goio_pps,GPIO::Direction::OUT);
//
//
//    // use a led to telling us that the machine is working now, quite stupid
//    std::function<void(void)> checkFunction = [&](void)->void{
//        while (1)
//        {
//            GPIO::gpio_set_value(gpio_check,GPIO::Pin::HIGH);
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//            GPIO::gpio_set_value(gpio_check,GPIO::Pin::LOW);
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//        }
//    };
//    std::thread checkThread(checkFunction);


    /// output the pps signal

    bool bSetHighGPIO(false),bSetLowGPIO(false);
    std::mutex mutexHighGPIO,mutexLowGPIO;

    // 1. a thread for timing
    std::function<void(void)> timingFunction = [&](void)->void {
        while(1){
            //get system time
            timeval tv;
            gettimeofday(&tv,NULL);
            if (tv.tv_usec <= 1)// time may be not accurate enough
            {
                mutexHighGPIO.lock();
                bSetHighGPIO = true;
                mutexHighGPIO.unlock();
            }
            if ( tv.tv_usec - 500*1000 == 0 || tv.tv_usec - 500*1000 == 1)// time may be not accurate enough
            {
                mutexLowGPIO.lock();
                bSetLowGPIO = true;
                mutexLowGPIO.unlock();
            }
        }

    };
    std::thread timingThread(timingFunction);

    // 2. a thread for output GPIO
    std::function<void(void)> outPutGPIOFunction = [&](void)->void {
        while(1)
        {
            if(bSetHighGPIO)
            {
                //set high gpio
                std::cout<<"set high gpio"<<std::endl;
                //GPIO::gpio_set_value(goio_pps,GPIO::Pin::HIGH);
                //wait for 10 us to guarantee the GPIO is set
                std::this_thread::sleep_for(std::chrono::microseconds(10));
                //send NMEA message
                timeval tv;
                gettimeofday(&tv,NULL);
                tm time = *localtime(&tv.tv_sec);
                //std::cout<<time.tm_sec<<": send NMEA message"<<std::endl;
                std::cout<<forgeNMEAMeaage(time)<<std::endl;

                mutexHighGPIO.lock();
                bSetHighGPIO = false;
                mutexHighGPIO.unlock();
            }
            if(bSetLowGPIO)
            {
                //set low gpio
                std::cout<<"set low"<<std::endl;
                //GPIO::gpio_set_value(goio_pps,GPIO::Pin::LOW);
                mutexHighGPIO.lock();
                bSetLowGPIO = false;
                mutexHighGPIO.unlock();
            }
        }

    };
    std::thread outPutGPIOThread(outPutGPIOFunction);

    timingThread.join();
    outPutGPIOThread.join();

//    checkThread.join();

//
//    GPIO::gpio_unexport(gpio_check);
//    GPIO::gpio_unexport(goio_pps);

    return 0;

}