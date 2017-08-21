//
// Created by jianping on 17-8-21.
//

#include <iostream>
#include <fstream>
#include <nmeaparse/nmea.h>

using namespace std;
using namespace nmea;

//$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76
std::string createNmeaMessage()
{
    NMEACommand message_GPGGA;
    message_GPGGA.name = "GPGGA";

    int hour = 9;
    int minute = 27;
    int sec = 50;
    float N = 5321.6802;
    float W = 00630.3372;
    int lock = 1;
    int numSat = 8;
    float coeff = 1.03;
    float height = 61.7;
    float gHeight= 55.2;

    char message[256];
    sprintf(message,"%02d%d%d.000,%08.4f,N,%010.4f,W,%d,%d,%.2f,%.1f,M,%.1f,M,,",
           hour,minute,sec,N,W,lock,numSat,coeff,height,gHeight);

    message_GPGGA.message = message;
    string nmea = message_GPGGA.toString();
    std::cout<<"a test GPGGA: "<< nmea<<std::endl;
    return nmea;
}

void parseString(std::string message_nmea)
{
    // Create a GPS service that will keep track of the fix data.
    NMEAParser parser;
    GPSService gps(parser);
    //gps.parser.log = true;		// true: will spit out all sorts of parse info on each sentence.

    // Handle events when the lock state changes
    gps.onLockStateChanged += [](bool newlock){
        if (newlock){
            cout << "\t\t\tGPS aquired LOCK!" << endl;
        }
        else {
            cout << "\t\t\tGPS lost lock. Searching..." << endl;
        }
    };

    // Handle any changes to the GPS Fix... This is called after onSentence
    gps.onUpdate += [&gps](){
        std::cout<<"\t\t\tget by parsing by string"<<std::endl;
        cout << "\t\t\tPosition: " << gps.fix.latitude << "'N, " << gps.fix.longitude << "'E" << endl;
        cout << "\t\t\tday: " << gps.fix.timestamp.day << ",  hour: " << gps.fix.timestamp.hour << ", minute: " <<
             gps.fix.timestamp.min <<", sec: "<< gps.fix.timestamp.sec<<", year: "<<gps.fix.timestamp.year<<endl << endl;
    };

    parser.readBuffer((uint8_t*)message_nmea.c_str(), message_nmea.size());
}


void parseByChar(std::string message_nmea)
{
    // Create a GPS service that will keep track of the fix data.
    NMEAParser parser;
    GPSService gps(parser);
    //gps.parser.log = true;		// true: will spit out all sorts of parse info on each sentence.

    // Handle events when the lock state changes
    gps.onLockStateChanged += [](bool newlock){
        if (newlock){
            cout << "\t\t\tGPS aquired LOCK!" << endl;
        }
        else {
            cout << "\t\t\tGPS lost lock. Searching..." << endl;
        }
    };

    // Handle any changes to the GPS Fix... This is called after onSentence
    gps.onUpdate += [&gps](){
        std::cout<<"\t\t\tget by parsing by char"<<std::endl;
        cout << "\t\t\tPosition: " << gps.fix.latitude << "'N, " << gps.fix.longitude << "'E" << endl;
        cout << "\t\t\tday: " << gps.fix.timestamp.day << ",  hour: " << gps.fix.timestamp.hour << ", minute: " <<
             gps.fix.timestamp.min <<", sec: "<< gps.fix.timestamp.sec<<", year: "<<gps.fix.timestamp.year<<endl << endl;
    };

    //parser.readBuffer((uint8_t*)message_nmea.c_str(), message_nmea.size());
    for (const auto& c : message_nmea) {
        parser.readByte(uint8_t(c));
    }
}

int main()
{
    std::string message_nmea = createNmeaMessage();

    parseString(message_nmea);
    parseByChar(message_nmea);

    return 0;
}