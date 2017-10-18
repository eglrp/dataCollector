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
        timeval tv;
        gettimeofday(&tv,NULL);
        ifs.read((char*)(&tv.tv_sec), sizeof(tv.tv_sec));
        ifs.read((char*)(&tv.tv_usec), sizeof(tv.tv_usec));

        IMU::InertialData inertialData;
        ifs >> inertialData;
        std::cout<<inertialData.accX<<" "<<inertialData.accY<<" "<<inertialData.accZ<<"\n";

    }
}