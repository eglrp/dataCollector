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
    std::cout<<sizeof(unsigned int)<<std::endl;
    std::string filename;
    std::cin>>filename;
    std::ifstream ifs(filename);
    std::ofstream ofs("imu.txt");
    while(ifs.good())
    {
        IMU::InertialData inertialData;
        timeval tv;
        gettimeofday(&tv,NULL);
        ifs.read((char*)(&tv.tv_sec), sizeof(tv.tv_sec));
        ifs.read((char*)(&tv.tv_usec), sizeof(tv.tv_usec));
        ifs >> inertialData;
        std::cout<<(int)inertialData.Year<<" "<<(int)inertialData.Month<<" "<<(int)inertialData.Day<<"\n";
        std::cout<<(int)inertialData.Hour<<" "<<(int)inertialData.Minute<<" "<<(int)inertialData.Second<<"\n";
        std::cout<<inertialData.ns<<"\n";
        std::cout<<inertialData.accX<<" "<<inertialData.accY<<" "<<inertialData.accZ<<"\n";
        ofs<<tv.tv_sec<<" , "<<tv.tv_usec<<std::endl;
    }
}