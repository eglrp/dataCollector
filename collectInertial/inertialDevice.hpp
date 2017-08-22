//
// Created by jianping on 17-8-22.
//

#ifndef DATACOLLECTOR_INERTIALDEVICE_HPP
#define DATACOLLECTOR_INERTIALDEVICE_HPP

#include <serial/serial.h>
#include <memory>
#include <sys/time.h>

namespace IMU{
    class InertialDivice{
    public:
        InertialDivice(std::string port, int baudRate=115200, float timeOut=1000)
        {
            m_pSerial = std::make_shared(port, baudRate, serial::Timeout::simpleTimeout(timeOut));
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
                gettimeofday(&tv_start,NULL);
                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
                waitFor(1);
                str = m_pSerial->read(1);

                if ((unsigned char)str[0] != 0xFA) //  0xFA: Preamble, indicator of start of packet
                    continue;

                waitFor(3);
                str = m_pSerial->read(3);
                if ((unsigned char)str[0] != 0xFF) // 0xFF: Bus identifier
                {
                    continue;
                }

                messageID = (unsigned char)str[1];
                unsigned short length = (unsigned char)str[2];
                if (length == 0xFF)
                {
                    waitFor(2);
                    str = m_pSerial->read(2);
                    memcpy(&length,&str[0],sizeof(length));
                }

                // read contents and checksum
                waitfor(length+1);
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
            return false;
        }

        bool parse_MTData2(const std::vector<unsigned char> &data)
        {
            unsigned short dataPos = 0;
            while (dataPos < data.size()-1)
            {
                //data id
                unsigned short dataId;
                memcpy(&dataId,&data[dataPos], sizeof(dataId));
                dataPos+=2;
                //float format
                char float_format;
                if((data_id&0x0003) == 0x3)
                {
                    float_format = 'd';
                }else if((data_id&0x0003) == 0x0){
                    float_format = 'f';
                } else{
                    std::cout<<"unknown float format\n";
                    return false;
                }
                //size
                unsigned char size;
                memcpy(&size,&data[dataPos], sizeof(size));
                dataPos+=1;

                //todo: parse


            }
        }


    private:
        bool waitFor(int size = 1)
        {
            timeval tv_start;;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms
            while (m_pSerial->available()<size)
            {
                timeval tv_now;
                gettimeofday(&tv_start,NULL);
                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
                if(diffTime > m_timeOut)
                {
                    return false;// time out
                }
            }
            return true;
        }



        std::shared_ptr<serial::Serial> m_pSerial;
        int m_timeOut;
    };
}

#endif //DATACOLLECTOR_INERTIALDEVICE_HPP
