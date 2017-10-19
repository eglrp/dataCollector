//
// Created by jianping on 17-10-19.
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
    IMU::InertialDivice device(config.IMUport);
    device._ensure_config_state();
    device.SetUTCTimeNow();
    device.SetUTCTimeNow();
    device.SetUTCTimeNow();
    device.GetUTCtime();
    device.GoToMeasure();
}