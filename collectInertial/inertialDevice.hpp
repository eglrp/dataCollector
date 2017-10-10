//
// Created by jianping on 17-8-22.
//

#ifndef DATACOLLECTOR_INERTIALDEVICE_HPP
#define DATACOLLECTOR_INERTIALDEVICE_HPP

#include <serial/serial.h>
#include <memory>
#include <sys/time.h>
#include <algorithm>
#include <ostream>
#include <istream>

namespace IMU{
    struct InertialData{
        //timeStamp
        unsigned short packetCount;
        //Sample Time Fine
        unsigned long fineSampleData;
        //Acceleration
        double accX,accY,accZ;
        //freeAcceleration
        double freeAccX,freeAccY,freeAccZ;
        //gyro
        double gyroX,gyroY,gyroZ;
        //Status word
        unsigned long statusWord;

        void write(std::ostream& os)
        {
            os.write(reinterpret_cast<char*> (&packetCount), sizeof(unsigned short));
            os.write(reinterpret_cast<char*> (&fineSampleData), sizeof(unsigned long));
            os.write(reinterpret_cast<char*> (&accX), sizeof(double));
            os.write(reinterpret_cast<char*> (&accY), sizeof(double));
            os.write(reinterpret_cast<char*> (&accZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccX), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccY), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroX), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroY), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&statusWord), sizeof(unsigned long));
        }
    };

    std::ostream& operator << (std::ostream& os, InertialData& data)
    {
        data.write(os);
        return os;
    }
    class InertialDivice{
    public:
        InertialDivice(std::string port, int baudRate=460800, uint32_t timeOut=1000)
        {
            m_pSerial = std::make_shared<serial::Serial>(port, baudRate, serial::Timeout::simpleTimeout(timeOut));
            m_pSerial->flushInput();
            m_pSerial->flushOutput();
            m_timeOut = timeOut;
        }

        //Low-level message receiving function.
        bool read_msg(unsigned char &messageID, std::vector<unsigned char> &data)
        {
            std::string str;

            timeval tv_start;;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms
            while (diffTime<m_timeOut)
            {
                timeval tv_now;
                gettimeofday(&tv_now,NULL);
                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
                if(!waitFor(1))
                {
                    std::cout<<"time out for Preamble, indicator of start of packet"<<std::endl;
                    continue;
                }
                str = m_pSerial->read(1);

                if ((unsigned char)str[0] != 0xFA) //  0xFA: Preamble, indicator of start of packet
                {
                    //std::cout<<"start"<<std::endl;
                    continue;
                }
                if(!waitFor(3))
                {
                    std::cout<<"time out for Bus identifier"<<std::endl;
                    continue;
                }
                str = m_pSerial->read(3);
                if ((unsigned char)str[0] != 0xFF) // 0xFF: Bus identifier
                {
                    std::cout<<"Bus identifier"<<std::endl;
                    continue;
                }

                messageID = (unsigned char)str[1];
                unsigned short length = (unsigned char)str[2];
                if (length == 0xFF)
                {
                    waitFor(2);
                    str = m_pSerial->read(2);
                    //memcpy(&length,&str[0],sizeof(length)); // wrong!!
                    length = (unsigned char)str[0]*0x100+(unsigned char)str[1];
                }

                // read contents and checksum
                //std::cout<<"length: "<<length<<std::endl;
                if(!waitFor(length+1))
                {
                    std::cout<<"time out for content"<<std::endl;
                    return false;
                }
                str = m_pSerial->read(length+1);
                if(str.length() < length+1)
                {
                    std::cout<<"unbelievable! I've waited for it!"<<std::endl;
                    continue;
                }
                unsigned char checksum = (unsigned char)*(str.end()-1);
                //std::vector<unsigned char> data;
                data.resize(length);
                memcpy(&data[0],&str[0],length);

                if (messageID == IMU::MIDs["Error"])
                {
                    std::cout<<"A error form imu"<<std::endl;
                    continue;
                }

                unsigned char sum(0);
                sum += 0xFF+messageID;
                if(length > 255)
                {
                    sum+=0xFF+length/0x100+length%0x100;
                } else{
                    sum+=length;
                }
                for (const auto& item : data) {
                    sum += item;
                }
                sum += checksum;
                if (0xFF & sum)
                {
                    std::cout<<"check sum is not zero"<<std::endl;
                    continue;
                }
                return true;
            }
            std::cout<<"time out"<<std::endl;
            return false;
        }

        bool parse_MTData2(const std::vector<unsigned char> &data)
        {
            unsigned short dataPos = 0;
            while (dataPos < data.size()-1)
            {
                //data id
                unsigned short dataId;
                //memcpy(&dataId,&data[dataPos], 2*sizeof(unsigned char)); //wrong!
                //dataId = (unsigned char)data[dataPos]*0x100+(unsigned char)data[dataPos+1];
                parseNumber(&data[dataPos],dataId);
                dataPos+=2;
                //float format
                char float_format;
                if((dataId&0x0003) == 0x3)
                {
                    float_format = 'd';
                }else if((dataId&0x0003) == 0x0){
                    float_format = 'f';
                } else{
                    std::cout<<"unknown float format\n";
                    return false;
                }
                //size
                unsigned char size;
                memcpy(&size,&data[dataPos], sizeof(size));
                dataPos+=1;

                //parse
                std::vector<unsigned char> values;
                values.resize(size);
                memcpy(&values[0],&data[dataPos], size*sizeof(unsigned char));
                dataPos+=size;

                unsigned int groupId = dataId&0xFF00;
                std::string groupName = IMU::getMDIGroupName(groupId);
                if(groupName.size() == 0)
                {
                    std::cout<<"unknown group name\n";
                    return false;
                }

                if(groupName.compare("Timestamp") == 0){
                    parseTimestamp(dataId,values);
                }
                else if(groupName.compare("Acceleration") == 0)
                {
                    parseAcceleration(dataId,values,float_format);
                }
                else if(groupName.compare("AngularVelocity") == 0)
                {
                    parseAngularVelocity(dataId,values,float_format);
                }
                else if(groupName.compare("Status") == 0)
                {
                    parseStatus(dataId,values);
                }
            }
        }
        InertialData m_inertialData;

    private:
        bool waitFor(int size = 1)
        {
            timeval tv_start;;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms
            while (m_pSerial->available()<size)
            {
                timeval tv_now;
                gettimeofday(&tv_now,NULL);
                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
                if(diffTime > size*m_timeOut)
                {
                    return false;// time out
                }

            }
            return true;
        }

        template <typename T>
        void parseNumber(const unsigned char* src, T& result)
        {
            std::vector<unsigned char> mem;
            mem.resize(sizeof(T));
            memcpy(&mem[0],src,sizeof(T));
            std::reverse(mem.begin(),mem.end());
            memcpy(&result,&mem[0],sizeof(T));
        }


        void parseTimestamp(const unsigned short& dataID, const std::vector<unsigned char>& values)
        {

            if((dataID&0x00F0) == 0x20) //Packet Counter //unsigned short
            {
                unsigned short result;
                parseNumber(&values[0],result);
                //std::cout<<"Packet Counter: "<<result<<std::endl;
                m_inertialData.packetCount = result;
            } else if((dataID&0x00F0) == 0x60) //Sample Time Fine //unsigned long
            {
                unsigned long result;
                parseNumber(&values[0],result);
                //std::cout<<"Sample Time Fine: "<<result<<std::endl;
                m_inertialData.fineSampleData = result;
            } else{
                std::cout<<"useless message"<<std::endl;
            }
        }


        void parseAcceleration(const unsigned short& dataID, const std::vector<unsigned char>& values, char floatFormat)
        {
            //assume that the float format is set to double... i am lazy.. forgive me
            if ((dataID&0x00F0) == 0x10)//Delta V
            {
                double dvX,dvY,dvZ;
                parseNumber(&values[0],dvX);
                parseNumber(&values[0+ sizeof(double)],dvY);
                parseNumber(&values[0+ 2*sizeof(double)],dvZ);
                //std::cout<<"dvx: "<<dvX<<", dvy: "<<dvY<<", dvz: "<<dvZ<<std::endl;
                //not used yet
            }
            else if ((dataID&0x00F0) == 0x20)//Acceleration
            {
                double accX,accY,accZ;
                parseNumber(&values[0],accX);
                parseNumber(&values[0+ sizeof(double)],accY);
                parseNumber(&values[0+ 2*sizeof(double)],accZ);
                //std::cout<<"accx: "<<accX<<", accy: "<<accY<<", accz: "<<accZ<<std::endl;
                m_inertialData.accX = accX;
                m_inertialData.accY = accY;
                m_inertialData.accZ = accZ;
            }
            else if ((dataID&0x00F0) == 0x30)//Free Acceleration
            {
                double free_accX,free_accY,free_accZ;
                parseNumber(&values[0],free_accX);
                parseNumber(&values[0+ sizeof(double)],free_accY);
                parseNumber(&values[0+ 2*sizeof(double)],free_accZ);
                //std::cout<<"free_accx: "<<free_accX<<", free_accy: "<<free_accY<<", free_accz: "<<free_accZ<<std::endl;
                m_inertialData.freeAccX = free_accX;
                m_inertialData.freeAccY = free_accY;
                m_inertialData.freeAccZ = free_accZ;
            }
            else{
                std::cout<<"useless information\n";
            }
        }

        void parseAngularVelocity(const unsigned short& dataID, const std::vector<unsigned char>& values, char floatFormat)
        {
            //assume that the float format is set to double... i am lazy.. forgive me
            if ((dataID&0x00F0) == 0x30)//Delta Q
            {
                double dq1,dq2,dq3,dq4;
                parseNumber(&values[0],dq1);
                parseNumber(&values[0+ sizeof(double)],dq2);
                parseNumber(&values[0+ 2*sizeof(double)],dq3);
                parseNumber(&values[0+ 3*sizeof(double)],dq4);
                //std::cout<<"dq1: "<<dq1<<", dq2: "<<dq2<<", dq3: "<<dq3<<", dq4: "<<dq4<<std::endl;
                //not used yet
            }
            else if ((dataID&0x00F0) == 0x20)//Rate of Turn
            {
                double gyrX,gyrY,gyrZ;
                parseNumber(&values[0],gyrX);
                parseNumber(&values[0+ sizeof(double)],gyrY);
                parseNumber(&values[0+ 2*sizeof(double)],gyrZ);
                //std::cout<<"gyrX: "<<gyrX<<", gyrY: "<<gyrY<<", gyrZ: "<<gyrZ<<std::endl;
                m_inertialData.gyroX = gyrX;
                m_inertialData.gyroY = gyrY;
                m_inertialData.gyroZ = gyrZ;
            }else{
                std::cout<<"useless information\n";
            }
        }

        void parseStatus(const unsigned short& dataID, const std::vector<unsigned char>& values)
        {
            if ((dataID&0x00F0) == 0x10)//Status Byte
            {
                unsigned char stateByte;
                parseNumber(&values[0],stateByte);
            }
            else if ((dataID&0x00F0) == 0x20)//Status Word
            {
                unsigned long stateWord;
                parseNumber(&values[0],stateWord);
                //std::cout<<"state word: "<<stateWord<<std::endl;
                m_inertialData.statusWord = stateWord;
            }
            else if ((dataID&0x00F0) == 0x40)//RSSI
            {
                signed char RSSI;
                parseNumber(&values[0],RSSI);
                std::cout<<"RSSI: "<<RSSI<<std::endl;
            }
            else{
                std::cout<<"useless information\n";
            }
        }
        std::shared_ptr<serial::Serial> m_pSerial;
        int m_timeOut;

    };
}

#endif //DATACOLLECTOR_INERTIALDEVICE_HPP
