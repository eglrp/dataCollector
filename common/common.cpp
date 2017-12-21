//
// Created by jianping on 17-8-25.
//

#include "common.hpp"
#include <fstream>

bool common::readConfig(common::sysConfig& config) {
    std::ifstream ifs(sysconfigFile);
    if (ifs.is_open())
    {
        cereal::XMLInputArchive xmlInputArchive(ifs);
        xmlInputArchive(cereal::make_nvp("systemConfig",config));
        if(config.bgood == false)
        {
            std::cout<<"check the sys config first!"<<std::endl;
            return false;
        }
        std::cout<<"load the sys config success!"<<std::endl;

        return true;
    } else{// create a default file
        std::cout<<"no sys config,we create a default one"<<std::endl;
        config.bgood = false;
        config.IMUport = "/dev/ttyUSB1";
        config.ArduPort = "/dev/ttyUSB0";
        config.IMU_triggerTimePath = std::string (sataFolderPath) + "imuTriggerTime.txt";
        config.cameraTimePath = std::string (sataFolderPath) + "cameraTime.txt";
        //config.IMU_dataTimePath = std::string (sataFolderPath) + "imuDataTime.txt";
        config.inertialPath = std::string (sataFolderPath) + "imuData.dat";
        config.imageFolderPath = std::string (sataFolderPath) + "image/";
        config.laser_dataPath = std::string (sataFolderPath) + "laser.dat";
        std::ofstream ofs(sysconfigFile);
        if (!ofs.is_open())
        {
            std::cout<<"please check permission! can not create sys config"<<std::endl;
            return false;
        }
        cereal::XMLOutputArchive xmlOutputArchive(ofs);
        xmlOutputArchive(cereal::make_nvp("systemConfig",config));
        return false;
    }

}