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

//serial
#include <serial/serial.h>
#include <fstream>

#include "common.hpp"


int main()
{

    //load sysconfig
    common::sysConfig config;
    if(!common::readConfig(config))
    {
        std::cout<<"read config failed\n";
        return -1;
    }

    bool bStop = false;
    std::ofstream ofsCameraTime(config.cameraTimePath);
    std::ofstream ofsIMUTime(config.IMU_triggerTimePath);
    if (ofsCameraTime.fail())
    {
        std::cout<<"fail to open cameraTimePath\n";
        return 0;
    }
    if (ofsIMUTime.fail())
    {
        std::cout<<"fail to open IMU_triggerTimePath\n";
        return 0;
    }

    /// register all the gpio
    //we use I2S0_SDO1/GPIO3_D6 for IMU synOUT
    //3x32+30 = 126
    const int gpio_IMU = 126;
    GPIO::gpio_export(gpio_IMU);
    GPIO::gpio_set_dir(gpio_IMU,GPIO::Direction::IN);

    bool bSetHighGPIOCamera(false);
    std::mutex mutexSetCameraGPIO;
    //0. a thread for receive IMUsynGpio
    unsigned int gpioIMUvalue(9999);
    unsigned int imuCount(0);
    std::function<void(void)> imuSynFunction = [&](void)->void {
        while(!bStop){
            unsigned int temp_gpioIMUvalue;
            GPIO::gpio_get_value(gpio_IMU,&temp_gpioIMUvalue);
            if(temp_gpioIMUvalue == 1 && gpioIMUvalue == 0){
                imuCount++;
                timeval tv;
                gettimeofday(&tv,NULL);
                ofsIMUTime<<tv.tv_sec<<","<<tv.tv_usec<<"\n";
            }
            if(imuCount == 40)
            {
                mutexSetCameraGPIO.lock();
                bSetHighGPIOCamera = true;
                mutexSetCameraGPIO.unlock();
                imuCount = 0;
                ofsIMUTime.flush();
            }
            gpioIMUvalue = temp_gpioIMUvalue;
        }
    };


    //we use GPIO3_D5 for output camera trigger
    //3*32+29 = 125
    const int goio_camera = 125;
    GPIO::gpio_export(goio_camera);
    GPIO::gpio_set_dir(goio_camera,GPIO::Direction::OUT);

    //0. a thread for trigger camera
    std::function<void(void)> cameraFunction = [&](void)->void {
        while(!bStop){
            if(bSetHighGPIOCamera)
            {
                GPIO::gpio_set_value(goio_camera,GPIO::Pin::HIGH);
                timeval tv;
                gettimeofday(&tv,NULL);
                //wait for 10 us to guarantee the GPIO is set
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                mutexSetCameraGPIO.lock();
                bSetHighGPIOCamera = false;
                mutexSetCameraGPIO.unlock();
                ofsCameraTime<<tv.tv_sec<<","<<tv.tv_usec<<"\n";
                //test
                //std::cout<<"camera triggered at:"<<tv.tv_usec<<std::endl;
                ofsCameraTime.flush();
                GPIO::gpio_set_value(goio_camera,GPIO::Pin::LOW);
            }

        }

    };
    std::thread cameraTriggerThread(cameraFunction);
    std::thread imuSynThread(imuSynFunction);


//    // use a led to telling us that the machine is working now, quite stupid
//    std::function<void(void)> checkFunction = [&](void)->void{
//        while (!bStop)
//        {
//            GPIO::gpio_set_value(gpio_check,GPIO::Pin::HIGH);
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//            GPIO::gpio_set_value(gpio_check,GPIO::Pin::LOW);
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//        }
//    };
//    std::thread checkThread(checkFunction);


    //we use I2S0_SDI1/GPIO3_D4 for output the GPS PPS
    //3x32+28 = 124
    const int goio_pps = 124;
    GPIO::gpio_export(goio_pps);
    GPIO::gpio_set_dir(goio_pps,GPIO::Direction::OUT);

    /// output the pps signal and nmea
    bool bSetHighGPIO(false),bSetLowGPIO(false);
    std::mutex mutexHighGPIO,mutexLowGPIO;

    // 1. a thread for timing
    std::function<void(void)> timingFunction = [&](void)->void {
        while(!bStop){
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



    // 2. a thread for output GPIO and NMEA
//    const std::string port_ttyS4 = "/dev/ttyS4";
//    const unsigned int baud_nmea = 9600;
//    serial::Serial serial_nmea(port_ttyS4, baud_nmea, serial::Timeout::simpleTimeout(1000));
//    if (!serial_nmea.isOpen())
//    {
//        std::cout<<"can not open serial "<<port_ttyS4<<std::endl;
//        return 0;
//    }
//
//
//    std::function<void(void)> outPutGPIOFunction = [&](void)->void {
//        while(!bStop)
//        {
//            if(bSetHighGPIO)
//            {
//                //set high gpio
//                //std::cout<<"set high gpio"<<std::endl;
//                GPIO::gpio_set_value(goio_pps,GPIO::Pin::HIGH);
//                //wait for 10 us to guarantee the GPIO is set
//                std::this_thread::sleep_for(std::chrono::microseconds(1000));
//                //send NMEA message
//                timeval tv;
//                gettimeofday(&tv,NULL);
//                tm time = *localtime(&tv.tv_sec);
//                //std::cout<<time.tm_sec<<": send NMEA message"<<std::endl;
//                //std::cout<<forgeNMEAMeaage(time)<<std::endl;
//                serial_nmea.write(forgeNMEAMeaage(time));
//
//                mutexHighGPIO.lock();
//                bSetHighGPIO = false;
//                mutexHighGPIO.unlock();
//            }
//            if(bSetLowGPIO)
//            {
//                //set low gpio
//                //std::cout<<"set low"<<std::endl;
//                GPIO::gpio_set_value(goio_pps,GPIO::Pin::LOW);
//                mutexHighGPIO.lock();
//                bSetLowGPIO = false;
//                mutexHighGPIO.unlock();
//            }
//        }
//
//    };
//    std::thread outPutGPIOThread(outPutGPIOFunction);

    timingThread.join();
//    outPutGPIOThread.join();
    //checkThread.join();

    std::cin.ignore(1024,'x');
    bStop = true;

//  release gpio
    GPIO::gpio_unexport(gpio_IMU);
    GPIO::gpio_unexport(goio_pps);

    return 0;

}