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

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QtCore/QTimer>
#include <QObject>
#include <QThread>
#include <QString>
#include <QDebug>

#include "glog/logging.h"

namespace IMU{
    struct InertialData{

        unsigned int ns;//unsigned int
        unsigned short Year;//unsigned short
        unsigned char Month;//unsigned char
        unsigned char Day;//unsigned char
        unsigned char Hour;//unsigned char
        unsigned char Minute;//unsigned char
        unsigned char Second;//unsigned char
        unsigned char Flags;//unsigned char

        //timeStamp
        unsigned short packetCount;
        //Sample Time Fine
        unsigned int fineSampleData;
        //Acceleration
        double accX,accY,accZ;
        //freeAcceleration
        double freeAccX,freeAccY,freeAccZ;
        //dv

        //gyro
        double gyroX,gyroY,gyroZ;
        //Status word
        unsigned int statusWord;
        //orientaiton
        double q0,q1,q2,q3;


        void write(std::ostream& os)
        {
            os.write(reinterpret_cast<char*> (&ns), sizeof(unsigned int));//unsigned int
            os.write(reinterpret_cast<char*> (&Year), sizeof(unsigned short));//unsigned short
            os.write(reinterpret_cast<char*> (&Month), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&Day), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&Hour), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&Minute), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&Second), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&Flags), sizeof(unsigned char));//unsigned char
            os.write(reinterpret_cast<char*> (&packetCount), sizeof(unsigned short));
            os.write(reinterpret_cast<char*> (&fineSampleData), sizeof(unsigned int));
            os.write(reinterpret_cast<char*> (&accX), sizeof(double));
            os.write(reinterpret_cast<char*> (&accY), sizeof(double));
            os.write(reinterpret_cast<char*> (&accZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccX), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccY), sizeof(double));
            os.write(reinterpret_cast<char*> (&freeAccZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroX), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroY), sizeof(double));
            os.write(reinterpret_cast<char*> (&gyroZ), sizeof(double));
            os.write(reinterpret_cast<char*> (&statusWord), sizeof(unsigned int));
            os.write(reinterpret_cast<char*> (&q0), sizeof(double));
            os.write(reinterpret_cast<char*> (&q1), sizeof(double));
            os.write(reinterpret_cast<char*> (&q2), sizeof(double));
            os.write(reinterpret_cast<char*> (&q3), sizeof(double));
        }

        void read(std::istream& is)
        {
            is.read(reinterpret_cast<char*> (&ns), sizeof(unsigned int));//unsigned int
            is.read(reinterpret_cast<char*> (&Year), sizeof(unsigned short));//unsigned short
            is.read(reinterpret_cast<char*> (&Month), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&Day), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&Hour), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&Minute), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&Second), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&Flags), sizeof(unsigned char));//unsigned char
            is.read(reinterpret_cast<char*> (&packetCount), sizeof(unsigned short));
            is.read(reinterpret_cast<char*> (&fineSampleData), sizeof(unsigned int));
            is.read(reinterpret_cast<char*> (&accX), sizeof(double));
            is.read(reinterpret_cast<char*> (&accY), sizeof(double));
            is.read(reinterpret_cast<char*> (&accZ), sizeof(double));
            is.read(reinterpret_cast<char*> (&freeAccX), sizeof(double));
            is.read(reinterpret_cast<char*> (&freeAccY), sizeof(double));
            is.read(reinterpret_cast<char*> (&freeAccZ), sizeof(double));
            is.read(reinterpret_cast<char*> (&gyroX), sizeof(double));
            is.read(reinterpret_cast<char*> (&gyroY), sizeof(double));
            is.read(reinterpret_cast<char*> (&gyroZ), sizeof(double));
            is.read(reinterpret_cast<char*> (&statusWord), sizeof(unsigned int));
            is.read(reinterpret_cast<char*> (&q0), sizeof(double));
            is.read(reinterpret_cast<char*> (&q1), sizeof(double));
            is.read(reinterpret_cast<char*> (&q2), sizeof(double));
            is.read(reinterpret_cast<char*> (&q3), sizeof(double));
        }
    };

    std::ostream& operator << (std::ostream& os, InertialData& data)
    {
        data.write(os);
        return os;
    }

    std::istream& operator >> (std::istream& is, InertialData& data)
    {
        data.read(is);
        return is;
    }
    class InertialDivice{
    public:
        InertialDivice(std::string port, int baudRate=460800, uint32_t timeOut=1000)
        {
//            m_pSerial = std::make_shared<serial::Serial>(port, baudRate, serial::Timeout::simpleTimeout(timeOut));
//            m_pSerial->flushInput();
//            m_pSerial->flushOutput();
            m_timeOut = timeOut;
            bconfig = false;
//            std::cout<<"create IMU from port :" <<port << "\n";

            qserial = std::make_shared<QSerialPort>();
            qserial->setPortName(port.c_str());
            if (qserial->open(QIODevice::ReadWrite)) {
                //qserial->setBaudRate(QSerialPort::Baud115200);
                qserial->setBaudRate(baudRate);
                qserial->setDataBits(QSerialPort::Data8);
                qserial->setParity(QSerialPort::NoParity);
                qserial->setStopBits(QSerialPort::OneStop);
                qserial->setFlowControl(QSerialPort::NoFlowControl);

                std::cout<<"create IMU from port :" <<port << "\n";
            } else{
                std::cout<<"fail to create IMU from port :" <<port << "\n";
                //LOG(ERROR)<<"fail to create IMU from port :" <<port << "\n";
            }
            qserial->flush();
        }

        //Low-level message receiving function.
        bool read_msg(unsigned char &messageID, std::vector<unsigned char> &data)
        {
            //std::string str;
            QByteArray b_str;

            timeval tv_start;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms
            while (diffTime < m_timeOut)
            {

                timeval tv_now;
                gettimeofday(&tv_now,NULL);
                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
                if(!waitFor(1))
                {
                    std::cout<<"time out for Preamble, indicator of start of packet"<<std::endl;
                    continue;
                }
                //str = m_pSerial->read(1);

                //str.resize(1);
                b_str = qserial->read(1);
                //qserial->read(&str[0],1);

                if ((unsigned char)b_str[0] != 0xFA) //  0xFA: Preamble, indicator of start of packet
                {
                    //std::cout<<"not start: "<<(int)str[0]<<std::endl;
                    continue;
                }

                if(!waitFor(3))
                {
                    std::cout<<"time out for Bus identifier"<<std::endl;
                    continue;
                }
                //str = m_pSerial->read(3);
//                str.resize(3);
//                qserial->read(&str[0],3);
                b_str = qserial->read(3);
                if ((unsigned char)b_str[0] != 0xFF) // 0xFF: Bus identifier
                {
                    std::cout<<"Bus identifier"<<std::endl;
                    continue;
                }

                messageID = (unsigned char)b_str[1];
                unsigned short length = (unsigned char)b_str[2];
                if (length == 0xFF)
                {
                    waitFor(2);
                    //str = m_pSerial->read(2);
//                    str.resize(2);
//                    qserial->read(&str[0],2);
                    b_str = qserial->read(2);
                    //memcpy(&length,&str[0],sizeof(length)); // wrong!!
                    length = (unsigned char)b_str[0]*0x100+(unsigned char)b_str[1];
                }

                // read contents and checksum

                if(!waitFor(length+1))
                {
                    std::cout<<"time out for content"<<std::endl;
                    return false;
                }
                //str = m_pSerial->read(length+1);
                //str.resize(length+1);
                //qserial->read(&str[0],length+1);
                b_str = qserial->read(length+1);
                if(b_str.length() < length+1)
                {
                    std::cout<<"unbelievable! I've waited for it!"<<std::endl;
                    continue;
                }
                unsigned char checksum = b_str[length];
                //std::vector<unsigned char> data;
                data.resize(length);
                //memcpy(&data[0],(unsigned char*)(&b_str[0]),length);
                for (int i = 0; i < length; ++i) {
                    data[i] = b_str[i];
                }

                if (messageID == IMU::MIDs["Error"])
                {
                    std::cout<<"A error from imu"<<std::endl;
                    continue;
                }

                unsigned char sum(0);
                sum += 0xFF+messageID;
                if(length > 255)
                {
                    std::cout<<"big data\n";
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
                    std::cout<<"check sum is not zero:"<< int(0xFF & sum )<<std::endl;
                    //continue;
                }
                return true;
            }
            std::cout<<"canot find message"<<std::endl;
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
                else if(groupName.compare("OrientationData") == 0)
                {
                    parse_orientation_data(dataId,values,float_format);
                }
            }
        }

        InertialData m_inertialData;


        // # High-level functions
        //Low-level message sending function
        void write_msg(unsigned char MID, std::vector<unsigned char> data)
        {
            unsigned short length = data.size();
            std::vector<unsigned char> lendat;
            std::vector<unsigned char> packet;
            packet.push_back(0xFA);
            packet.push_back(0xFF);
            packet.push_back(MID);
            if(length > 254)
            {
                lendat.resize(1+sizeof(unsigned short));
                lendat[0] = 0xFF;
                packNumber((unsigned char*)&lendat[1],length);
                packet.push_back(lendat[1]);
                packet.push_back(lendat[2]);
            }
            else
            {
                lendat.resize(1);
                unsigned char l = length;
                packet.push_back(l);
            }
            for (int j = 0; j < data.size(); ++j) {
                packet.push_back(data[j]);
            }

            unsigned char sum = 0;
            for (int i = 1; i < packet.size(); ++i) {
                sum += packet[i];
            }
            sum*=-1;
            unsigned char checksum = 0xFF & sum;
            packet.push_back(checksum);
            //while (((time.time()-start) < self.timeout) && self.device.read())
            //    ;

            timeval tv_start;;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms
            //while (m_pSerial->available() > 0)
//            while (qserial->bytesAvailable() > 0)
//            {
//                //std::cout<<qserial->bytesAvailable()<<std::endl;
//                timeval tv_now;
//                gettimeofday(&tv_now,NULL);
//                diffTime = 1000*(tv_now.tv_sec-tv_start.tv_sec)+(tv_now.tv_usec-tv_start.tv_usec)/1000;
//                if(diffTime > m_timeOut)
//                {
//                    std::cout<<"time out"<<std::endl;
//                    return;// time out
//                }
//            }


//            m_pSerial->flushInput();
//            m_pSerial->flushOutput();
//            m_pSerial->write(packet);
            qserial->flush();
            qserial->write((char*)&packet[0],packet.size());

        }

        //Send a message and read confirmation.
        std::vector<unsigned char> write_ack(unsigned char MID, std::vector<unsigned char> data)
        {
            write_msg(MID,data);
            unsigned char messageID_act;
            std::vector<unsigned char> data_act;
            for (int i = 0; i < 2; ++i) {
                read_msg(messageID_act,data_act);
                if(messageID_act == MID+1)
                {
                    break;
                } else{
                    std::cout<<"ack "<< int(MID+1) << " expected, got "<<(int) messageID_act <<" instead\n";
                }
            }
            return data_act;
        }
        //Switch device to config state if necessary.
        void _ensure_config_state()
        {
            if(bconfig)
            {
                ;
            } else{
                GoToConfig();
                bconfig = true;
            }
        }



        void GetUTCtime()
        {
            _ensure_config_state();
            std::string data;
            unsigned int ns;
            unsigned short year;
            unsigned char month;
            unsigned char day;
            unsigned char hour;
            unsigned char minute;
            unsigned char second;
            unsigned char flag;
            std::vector<unsigned char> act_data =
                    write_ack(MIDs["SetUTCTime"], std::vector<unsigned char>());

            int step = 0;
            parseNumber(&act_data[step],ns);
            step+=sizeof(ns);
            parseNumber(&act_data[step],year);
            step+=sizeof(year);
            month = act_data[step];
            step++;
            day = act_data[step];
            step++;
            hour = act_data[step];
            step++;
            minute = act_data[step];
            step++;
            second = act_data[step];
            step++;
            flag = act_data[step];
            step++;

            std::cout<<"ns:"<<(unsigned int)ns<<" ";
            std::cout<<"year:"<<(unsigned int)year<<" ";
            std::cout<<"month:"<<(unsigned int)month<<" ";
            std::cout<<"day:"<<(unsigned int)day<<" ";
            std::cout<<"hour:"<<(unsigned int)hour<<" ";
            std::cout<<"minute:"<<(unsigned int)minute<<" ";
            std::cout<<"second:"<<(unsigned int)second<<" ";
            std::cout<<"flag:"<<(unsigned int)flag<<std::endl;

        }

        void SetUTCTimeNow()
        {
            _ensure_config_state();
            std::vector<unsigned char> data;
            unsigned int ns;
            unsigned short year;
            unsigned char month;
            unsigned char day;
            unsigned char hour;
            unsigned char minute;
            unsigned char second;
            unsigned char flag;
            data.resize(12);
            //packNumber(&ns,)
//            data = struct.pack('!IHBBBBBB', ns, year, month, day, hour, minute,
//                               second, flag)  # no clue what setting flag can mean
            timeval tv;
            gettimeofday(&tv,NULL);
            ns = tv.tv_usec*1000;
            struct tm *p;
            p = gmtime(&tv.tv_sec);
            year = p->tm_year+1900;

            month = p->tm_mon+1;
            day = p->tm_mday;
            hour = p->tm_hour+8;
            minute = p->tm_min;
            second = p->tm_sec;
            flag = 1;


            packNumber(&data[0],ns);
            packNumber(&data[4],year);

            data[6] = (month);
            data[7] =(day);
            data[8] =(hour);
            data[9] =(minute);
            data[10] =(second);
            data[11] =(flag);
            write_ack(MIDs["SetUTCTime"], data);
        }
        void GoToMeasure()
        {
            _ensure_config_state();
            write_ack(MIDs["GoToMeasurement"], std::vector<unsigned char>());
            bconfig = false;
        }

    private:
        void GoToConfig()
        {
            write_ack(MIDs["GoToConfig"],std::vector<unsigned char>());
        }

        bool waitFor(int size = 1)
        {
            timeval tv_start;;
            gettimeofday(&tv_start,NULL);
            long diffTime(0);//ms

            //while (m_pSerial->available()<size)
            while (qserial->bytesAvailable()<size)
            {
                qserial->waitForReadyRead(1);
                //std::cout<<qserial->bytesAvailable()<<std::endl;
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

        template <typename T>
        void packNumber(unsigned char* result, T input)
        {
            std::vector<unsigned char> mem;
            mem.resize(sizeof(T));
            memcpy(&mem[0],(unsigned char*)&input,sizeof(T));
            std::reverse(mem.begin(),mem.end());
            memcpy(result,&mem[0],sizeof(T));
        }


        void parseTimestamp(const unsigned short& dataID, const std::vector<unsigned char>& values)
        {
//            if (data_id&0x00F0) == 0x10:	# UTC Time
//            o['ns'], o['Year'], o['Month'], o['Day'], o['Hour'],\
//						o['Minute'], o['Second'], o['Flags'] =\
//						struct.unpack('!LHBBBBBB', content)

            if((dataID&0x00F0) == 0x10)// UTC time
            {
                int step = 0;
                unsigned int ns;//unsigned int
                parseNumber(&values[step],ns);
                m_inertialData.ns = ns;
                step += sizeof(ns);

                unsigned short Year;//unsigned short
                parseNumber(&values[step],Year);
                m_inertialData.Year = Year;
                step+= sizeof(Year);

                unsigned char Month;//unsigned char
                parseNumber(&values[step],Month);
                m_inertialData.Month = Month;
                step+= sizeof(Month);

                unsigned char Day;//unsigned char
                parseNumber(&values[step],Day);
                m_inertialData.Day = Day;
                step+= sizeof(Day);

                unsigned char Hour;//unsigned char
                parseNumber(&values[step],Hour);
                m_inertialData.Hour = Hour;
                step+= sizeof(Hour);

                unsigned char Minute;//unsigned char
                parseNumber(&values[step],Minute);
                m_inertialData.Minute = Minute;
                step+= sizeof(Minute);

                unsigned char Second;//unsigned char
                parseNumber(&values[step],Second);
                m_inertialData.Second = Second;
                step+= sizeof(Second);

                unsigned char Flags;//unsigned char
                parseNumber(&values[step],Flags);
                m_inertialData.Flags = Flags;
                step+= sizeof(Flags);
            }
            else if((dataID&0x00F0) == 0x20) //Packet Counter //unsigned short
            {
                unsigned short result;
                parseNumber(&values[0],result);
                //std::cout<<"Packet Counter: "<<result<<std::endl;
                m_inertialData.packetCount = result;
            } else if((dataID&0x00F0) == 0x60) //Sample Time Fine //unsigned int
            {
                unsigned int result;
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
                unsigned int stateWord;
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

        void parse_orientation_data(const unsigned short& dataID, const std::vector<unsigned char>& values, char floatFormat)
        {
            //assume that the float format is set to double... i am lazy.. forgive me
            if ((dataID&0x00F0) == 0x10)// Quaternion
            {
                double q0,q1,q2,q3;
                parseNumber(&values[0],q0);
                parseNumber(&values[0+ sizeof(double)],q1);
                parseNumber(&values[0+ 2*sizeof(double)],q2);
                parseNumber(&values[0+ 3*sizeof(double)],q3);

                m_inertialData.q0 = q0;
                m_inertialData.q1 = q1;
                m_inertialData.q2 = q2;
                m_inertialData.q3 = q3;
            }
            else if ((dataID&0x00F0) == 0x20)//Rotation Matrix
            {
                //LOG(ERROR)<<"MTI orientation is set to rotation matrix!! ";
            }
            else if ((dataID&0x00F0) == 0x30)//Euler Angles
            {

            }
            else{
                std::cout<<"useless information\n";
            }
        }


        //std::shared_ptr<serial::Serial> m_pSerial;
        //serial
        std::shared_ptr<QSerialPort> qserial;
        int m_timeOut;
        bool bconfig;

    };
}

#endif //DATACOLLECTOR_INERTIALDEVICE_HPP
