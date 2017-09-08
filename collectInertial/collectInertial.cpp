//
// Created by jianping on 17-8-22.
//

#include <iostream>
#include <string.h>
#include "messageDef.hpp"
#include "inertialDevice.hpp"
#include <fstream>
#include <common.hpp>



int main()
{
    //load sysconfig
    common::sysConfig config;
    if(!common::readConfig(config))
    {
        std::cout<<"read config failed\n";
        return -1;
    }
    std::cout<<"the inertial folder is :"<<config.inertialPath<<std::endl;
    IMU::InertialDivice device(config.IMUport);
    unsigned char messageID;
    std::vector<unsigned char> data;
    std::ofstream ofs(config.inertialPath,std::ios_base::out|std::ios_base::binary);
    while(1)
    {
        if(!device.read_msg(messageID,data))
        {
            continue;
        }
        device.parse_MTData2(data);
        ofs << device.m_inertialData;
    }


    return 0;
}