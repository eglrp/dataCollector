#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PacketDriver.h"
#include "PacketDecoder.h"
#include <boost/shared_ptr.hpp>
#include<deque>
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
  
  int c = 0;
  while (c<1000) {
    driver.GetPacket(data, dataLength);
    decoder.DecodePacket(data, dataLength);
    frames = decoder.GetFrames();
    if (decoder.GetLatestFrame(&latest_frame)) {
      for(int i = 0; i < latest_frame.x.size(); i++)
      {
          ofs<<latest_frame.x[i]<<" "<<latest_frame.y[i]<<" "<<latest_frame.z[i]<<"\n";
      }
      std::cout << "Number of points: " << latest_frame.x.size() << std::endl;
      c++;
    }
  }
  ofs.close();

  return 0;
}
