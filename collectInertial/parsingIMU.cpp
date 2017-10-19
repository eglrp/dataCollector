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

    long last =0;
    while(ifs.good())
    {
        IMU::InertialData inertialData;
        timeval tv;
        gettimeofday(&tv,NULL);
        ifs >> inertialData;
        ofs<<inertialData.Hour<<" "<<inertialData.Minute<<" "<<inertialData.Second
           <<inertialData.ns<<std::endl;
    }
}