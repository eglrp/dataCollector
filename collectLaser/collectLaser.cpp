//
// Created by jianping on 17-9-9.
//

#include <iostream>
#include "PacketDriver.h"
#include "PacketDecoder.h"
#include <fstream>
#include "common.hpp"

using namespace std;

int main()
{
    //load sysconfig
    common::sysConfig config;
    if(!common::readConfig(config))
    {
        std::cout<<"read config failed\n";
        return -1;
    }
    PacketDriver driver;
    driver.InitPacketDriver(DATA_PORT);
    PacketDecoder decoder;
    decoder.SetCorrectionsFile("16db.xml");
    std::ofstream ofs(config.laser_dataPath,std::ios_base::out|std::ios_base::binary);

    std::string* data = new std::string();
    unsigned int* dataLength = new unsigned int();
    std::deque<PacketDecoder::HDLFrame> frames;
    PacketDecoder::HDLFrame latest_frame;

    int count = 0;
    while (1) {
        driver.GetPacket(data, dataLength);
        timeval tv;
        gettimeofday(&tv,NULL);
        decoder.DecodePacket(data, dataLength);
        frames = decoder.GetFrames();
        if (decoder.GetLatestFrame(&latest_frame)) {
            //ofs<<"f"<<tv.tv_sec<<" "<<tv.tv_usec<<"\n";
            ofs.write((char*)(&tv.tv_sec), sizeof(tv.tv_sec));
            ofs.write((char*)(&tv.tv_usec), sizeof(tv.tv_usec));
            int npts = latest_frame.x.size();
            ofs.write((char*)(&npts), sizeof(npts));
            for(int i = 0; i < npts; i++)
            {
                //ofs<<latest_frame.ms_from_top_of_hour[i]<<" "
                //        <<latest_frame.azimuth[i]<<" "
                //   <<latest_frame.x[i]<<" "<<latest_frame.y[i]<<" "<<latest_frame.z[i]<<" "<<(int)latest_frame.intensity[i]<<"\n";
                ofs.write((char*)(&latest_frame.ms_from_top_of_hour[i]),sizeof(latest_frame.ms_from_top_of_hour[i]));
                ofs.write((char*)(&latest_frame.x[i]),sizeof(latest_frame.x[i]));
                ofs.write((char*)(&latest_frame.y[i]),sizeof(latest_frame.y[i]));
                ofs.write((char*)(&latest_frame.z[i]),sizeof(latest_frame.z[i]));
                ofs.write((char*)(&latest_frame.intensity[i]),sizeof(latest_frame.intensity[i]));
            }
            count ++;
            if(count == 10)
            {
                count = 0;
                ofs.flush();
            }

        }
    }
    ofs.close();

    return 0;
}
