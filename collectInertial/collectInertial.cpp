//
// Created by jianping on 17-8-22.
//

#include <iostream>
#include <string.h>
#include "messageDef.hpp"
#include "inertialDevice.hpp"

int main()
{
    IMU::InertialDivice device("/dev/ttyUSB0");
    unsigned char messageID;
    std::vector<unsigned char> data;
    while(1)
    {
        if(!device.read_msg(messageID,data))
        {
            continue;
        }
        std::cout<<"messageId :"<<int (messageID)<<std::endl;
        device.parse_MTData2(data);
    }


    return 0;
}