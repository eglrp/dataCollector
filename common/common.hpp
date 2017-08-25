//
// Created by jianping on 17-8-25.
//

#ifndef DATACOLLECTOR_COMMON_HPP
#define DATACOLLECTOR_COMMON_HPP

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

#define sysconfigFile "./sysconfig.xml"


namespace common{
    struct sysConfig{
        std::string imageFolderPath;
        std::string inertialPath;
        bool bgood;

        template <class Archive>
        void serialize( Archive & ar )
        {
            ar(  cereal::make_nvp("imageFolderPath",imageFolderPath),
                 cereal::make_nvp("inertialPath",inertialPath),
                 cereal::make_nvp("bgood",bgood));
        }
    };

    bool readConfig(common::sysConfig& config);
}


#endif //DATACOLLECTOR_COMMON_HPP
