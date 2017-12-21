//
// Created by jianping on 17-12-20.
//

//serial
#include <serial/serial.h>
#include <fstream>

#include "common.hpp"
#include "arduCommunicator.h"
#include "QCoreApplication"
int main(int argc,char** argv)
{
    QCoreApplication application(argc, argv);
    common::sysConfig config;
    if(!common::readConfig(config))
    {
        qDebug("read config failed\n");
        return -1;
    }
    std::ofstream ofsCameraTime(config.cameraTimePath);
    if (ofsCameraTime.fail())
    {
        qDebug("fail to open cameraTimePath");
        return 0;
    }
    ArduCommunicator arduCommunicator;
    arduCommunicator.setUp(config.ArduPort);
    arduCommunicator.getTime();
    arduCommunicator.startMeasure(ofsCameraTime);
    application.exec();

}