//
// Created by jianping on 17-10-15.
//
#include <iostream>
#include <string.h>
#include "messageDef.hpp"
#include "inertialDevice.hpp"
#include <fstream>
#include <common.hpp>

int main()
{
    std::string filename;
    std::cin>>filename;
    std::ifstream ifs(filename);
    std::ofstream ofs("imu.txt");
    while(ifs.good())
    {
        IMU::InertialData inertialData;
        ifs >> inertialData;
        std::cout<<inertialData.Year<<" "<<inertialData.Month<<" "<<inertialData.Day<<"\n";
        std::cout<<inertialData.Hour<<" "<<inertialData.Minute<<" "<<inertialData.Second<<"\n";
        std::cout<<inertialData.ns<<"\n";
        std::cout<<inertialData.accX<<" "<<inertialData.accY<<" "<<inertialData.accZ<<"\n";

    }
}