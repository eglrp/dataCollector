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
    unsigned long start_sec,start_usec;
    unsigned long temp_sec,temp_usec;
    while(ifs.good())
    {
        IMU::InertialData inertialData;
        timeval tv;
        gettimeofday(&tv,NULL);
        ifs >> inertialData;
        ofs<<(int)inertialData.Hour<<" "<<(int)inertialData.Minute<<" "<<(int)inertialData.Second<<
           " "<<(unsigned int)inertialData.ns<<std::endl;
        //char line[256];
        //ifs.getline(line,256);
        //sscanf(line,"%ld,%ld",&temp_sec,&temp_usec);
        //std::cout<<temp_usec-start_usec<<"\n";
        //start_usec = temp_usec;
    }
}