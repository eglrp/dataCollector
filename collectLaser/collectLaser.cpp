//
// Created by jianping on 17-9-9.
//

#include <iostream>
#include "PacketDriver.h"
#include "PacketDecoder.h"
#include <fstream>

using namespace std;

int main()
{
    PacketDriver driver;
    driver.InitPacketDriver(DATA_PORT);
    PacketDecoder decoder;
    decoder.SetCorrectionsFile("16db.xml");
    std::ofstream ofs("./data.txt");

    std::string* data = new std::string();
    unsigned int* dataLength = new unsigned int();
    std::deque<PacketDecoder::HDLFrame> frames;
    PacketDecoder::HDLFrame latest_frame;

    while (1) {
        driver.GetPacket(data, dataLength);
        timeval tv;
        gettimeofday(&tv,NULL);
        decoder.DecodePacket(data, dataLength);
        frames = decoder.GetFrames();
        if (decoder.GetLatestFrame(&latest_frame)) {
            ofs<<tv.tv_sec<<" "<<tv.tv_usec<<"\n";
            for(int i = 0; i < latest_frame.x.size(); i++)
            {
                ofs<<latest_frame.ms_from_top_of_hour[i]<<" "
                   <<latest_frame.x[i]<<" "<<latest_frame.y[i]<<" "<<latest_frame.z[i]<<" "<<latest_frame.intensity[i]<<"\n";
            }
            ofs.flush();
        }
    }
    ofs.close();

    return 0;
}
