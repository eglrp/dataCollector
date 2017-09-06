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
        config.IMUport = "/dev/ttyUSB0";
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