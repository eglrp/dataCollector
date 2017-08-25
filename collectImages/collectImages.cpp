//
// Created by jianping on 17-8-24.
//

#include "FlyCapAPI.hpp"
#include "common.hpp"

int main(int argc, char**argv)
{

    //load sysconfig
    common::sysConfig config;
    if(!common::readConfig(config))
    {
        std::cout<<"read config failed\n";
        return -1;
    }
    std::cout<<"the image folder is :"<<config.imageFolderPath<<std::endl;

    FlyCapture2::Camera cam;
    if(!FLYCAP_API::initCamera(cam))
    {
        return false;
    }
    std::cout<<"finish init"<<std::endl;

    FlyCapture2::Error error;
    while(1)
    {
        // Grab image
        FlyCapture2::Image image;
        error = cam.RetrieveBuffer(&image);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            FLYCAP_API::PrintError(error);
            break;
        }
        // Create a converted image
        FlyCapture2::Image convertedImage;

        // Convert the raw image
        error = image.Convert(FlyCapture2::PIXEL_FORMAT_RGB8, &convertedImage);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            FLYCAP_API::PrintError(error);
            return -1;
        }

        // Create a unique filename

        std::cout<<"write image"<<std::endl;
        std::ostringstream filename;
        filename << config.imageFolderPath <<image.GetTimeStamp().seconds<<"-" <<image.GetTimeStamp().microSeconds<< "-"
                 << ".jpg";

        // Save the image. If a file format is not passed in, then the file
        // extension is parsed to attempt to determine the file format.
        error = convertedImage.Save(filename.str().c_str());
        if (error != FlyCapture2::PGRERROR_OK)
        {
            FLYCAP_API::PrintError(error);
            return -1;
        }
    }

    if(!FLYCAP_API::releaseCamera(cam))
    {
        return false;
    }

    return 0;
}