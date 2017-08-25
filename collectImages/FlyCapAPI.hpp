//
// Created by jianping on 17-8-25.
//

#ifndef DATACOLLECTOR_FLYCAPAPI_HPP
#define DATACOLLECTOR_FLYCAPAPI_HPP
#include <sstream>
#include <iostream>
#include <flycapture/FlyCapture2.h>

namespace FLYCAP_API{
    void PrintError(FlyCapture2::Error error) { error.PrintErrorTrace(); }

    bool PollForTriggerReady(FlyCapture2::Camera *pCam)
    {
        const unsigned int k_softwareTrigger = 0x62C;
        FlyCapture2::Error error;
        unsigned int regVal = 0;

        do
        {
            error = pCam->ReadRegister(k_softwareTrigger, &regVal);
            if (error != FlyCapture2::PGRERROR_OK)
            {
                PrintError(error);
                return false;
            }

        } while ((regVal >> 31) != 0);

        return true;
    }

    bool initCamera(FlyCapture2::Camera& cam)
    {
        FlyCapture2::Error error;

        FlyCapture2::BusManager busMgr;
        unsigned int numCameras;
        error = busMgr.GetNumOfCameras(&numCameras);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        std::cout << "Number of cameras detected: " << numCameras << std::endl;

        if (numCameras < 1)
        {
            std::cout << "Insufficient number of cameras... exiting" << std::endl;
            return false;
        }

        FlyCapture2::PGRGuid guid;
        error = busMgr.GetCameraFromIndex(0, &guid);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        //FlyCapture2::Camera cam;

        // Connect to a camera
        error = cam.Connect(&guid);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Power on the camera
        const unsigned int k_cameraPower = 0x610;
        const unsigned int k_powerVal = 0x80000000;
        error = cam.WriteRegister(k_cameraPower, k_powerVal);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        const unsigned int millisecondsToSleep = 100;
        unsigned int regVal = 0;
        unsigned int retries = 10;

        // Wait for camera to complete power-up
        do
        {
            struct timespec nsDelay;
            nsDelay.tv_sec = 0;
            nsDelay.tv_nsec = (long)millisecondsToSleep * 1000000L;
            nanosleep(&nsDelay, NULL);
            error = cam.ReadRegister(k_cameraPower, &regVal);
            if (error == FlyCapture2::PGRERROR_TIMEOUT)
            {
                // ignore timeout errors, camera may not be responding to
                // register reads during power-up
            }
            else if (error != FlyCapture2::PGRERROR_OK)
            {
                PrintError(error);
                return false;
            }

            retries--;
        } while ((regVal & k_powerVal) == 0 && retries > 0);

        // Check for timeout errors after retrying
        if (error == FlyCapture2::PGRERROR_TIMEOUT)
        {
            PrintError(error);
            return false;
        }

        // Get the camera information
        FlyCapture2::CameraInfo camInfo;
        error = cam.GetCameraInfo(&camInfo);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Get current trigger settings
        FlyCapture2::TriggerMode triggerMode;
        error = cam.GetTriggerMode(&triggerMode);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Set camera to trigger mode 0
        triggerMode.onOff = true;
        triggerMode.mode = 0;
        triggerMode.parameter = 0;

        // Triggering the camera externally using source :gpio2
        triggerMode.source = 2;

        error = cam.SetTriggerMode(&triggerMode);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Poll to ensure camera is ready
        bool retVal = PollForTriggerReady(&cam);
        if (!retVal)
        {
            std::cout << std::endl;
            std::cout << "Error polling for trigger ready!" << std::endl;
            return -1;
        }
        // Get the camera configuration
        FlyCapture2::FC2Config config;
        error = cam.GetConfiguration(&config);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Set the grab timeout to 5 seconds
        config.grabTimeout = 5000;

        // Set the camera configuration
        error = cam.SetConfiguration(&config);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Camera is ready, start capturing images
        error = cam.StartCapture();
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }
        return true;
    }

    bool releaseCamera(FlyCapture2::Camera& cam)
    {
        // Turn trigger mode off.
        FlyCapture2::TriggerMode triggerMode;
        FlyCapture2::Error error;
        triggerMode.onOff = false;
        error = cam.SetTriggerMode(&triggerMode);
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }
        std::cout << std::endl;
        std::cout << "Finished grabbing images" << std::endl;

        // Stop capturing images
        error = cam.StopCapture();
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }

        // Disconnect the camera
        error = cam.Disconnect();
        if (error != FlyCapture2::PGRERROR_OK)
        {
            PrintError(error);
            return false;
        }
    }
}


#endif //DATACOLLECTOR_FLYCAPAPI_HPP
