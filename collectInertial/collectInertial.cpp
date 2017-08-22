//
// Created by jianping on 17-8-22.
//

#include <iostream>
#include <string.h>
#include "messageDef.hpp"

int main()
{
//    for (const auto& value : IMU::MIDs)
//    {
//        std::cout<<value.first<<" : "<<std::hex <<value.second<<std::endl;
//    }
    unsigned short a = 0x1520;
    std::cout<<a<<std::endl;
    unsigned char b[2];
    memcpy(&b[0],&a,sizeof(unsigned short));
    std::cout<<(int)b[1]<<std::endl;

    return 0;
}