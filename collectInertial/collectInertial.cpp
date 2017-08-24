//
// Created by jianping on 17-8-22.
//

#include <iostream>
#include <string.h>
#include "messageDef.hpp"
#include "inertialDevice.hpp"
#include <fstream>

int main()
{
    IMU::InertialDivice device("/dev/ttyUSB0");
    unsigned char messageID;
    std::vector<unsigned char> data;
    std::ofstream ofs("./imuData.dat",std::ios_base::out|std::ios_base::binary);
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