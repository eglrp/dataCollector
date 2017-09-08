//
// Created by jianping on 17-8-25.
//

#ifndef DATACOLLECTOR_COMMON_HPP
#define DATACOLLECTOR_COMMON_HPP

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#define sysconfigFile "./sysconfig.xml"
#define sataFolderPath "/media/xx/xx/datacollector/"

namespace common{
    struct sysConfig{
        std::string imageFolderPath;
        std::string cameraTimePath;
        std::string inertialPath;
        std::string IMUport; //default is /dev/ttyUSB0
        std::string IMU_dataTimePath;
        std::string IMU_triggerTimePath;
        bool bgood;

        template <class Archive>
        void serialize( Archive & ar )
        {
            ar(  cereal::make_nvp("imageFolderPath",imageFolderPath),
                 cereal::make_nvp("cameraTimePath",cameraTimePath),
                 cereal::make_nvp("inertialPath",inertialPath),
                 cereal::make_nvp("IMU_port",IMUport),
                 cereal::make_nvp("IMU_dataTimePath",IMU_dataTimePath),
                 cereal::make_nvp("IMU_triggerTimePath",IMU_triggerTimePath),
                 cereal::make_nvp("bgood",bgood)
            );
        }
    };

    bool readConfig(common::sysConfig& config);
}


#endif //DATACOLLECTOR_COMMON_HPP
