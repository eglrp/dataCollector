//
// Created by jianping on 17-8-21.
//

#ifndef DATACOLLECTOR_FORGENMEA_HPP
#define DATACOLLECTOR_FORGENMEA_HPP

#include <time.h>
#include <string.h>
#include <nmeaparse/nmea.h>

std::string forgeNMEAMeaage(const tm& time)
{
    nmea::NMEACommand message_GPGGA;
    message_GPGGA.name = "GPGGA";

    int hour = time.tm_hour;
    int minute = time.tm_min;
    int sec = time.tm_sec;
    double N = 5321.6802;
    double W = 00630.3372;
    int lock = 1;
    int numSat = 8;
    float coeff = 1.03;
    float height = 61.7;
    float gHeight= 55.2;

    char message[256];
    sprintf(message,"%02d%d%d.000,%08.4f,N,%010.4f,W,%d,%d,%.2f,%.1f,M,%.1f,M,,",
            hour,minute,sec,N,W,lock,numSat,coeff,height,gHeight);

    message_GPGGA.message = message;
    std::string nmea = message_GPGGA.toString();
    return nmea;
}




#endif //DATACOLLECTOR_FORGENMEA_HPP
