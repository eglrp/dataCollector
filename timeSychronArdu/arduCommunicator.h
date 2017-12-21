//
// Created by jianping on 17-12-20.
//

#ifndef DATACOLLECTOR_ARDUCOMMUNICATOR_H
#define DATACOLLECTOR_ARDUCOMMUNICATOR_H
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <memory>
#include <fstream>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QtCore/QTimer>
#include <QObject>
#include <QThread>
#include <QString>
#include <QDebug>

class ArduCommunicator{
public:

    const unsigned int baudRate= 115200;
    ArduCommunicator();

    bool setUp(std::string port_name)
    {
        _portName = port_name;
        if(!setUpSerial())
        {
            return false;
        }
        //check for ardu ready
        while (1){
            serial->write(QByteArray("1"));
            QByteArray t_data = readData();
            if(t_data.size() > 0)
            {
                break;
            }
            usleep(10000);
        }
        qDebug("Aduino ready.");
    }

    bool getTime();
    bool startMeasure(std::ofstream &ofs);


private:
    QByteArray readData();
    double readMeasurement();

    bool setUpSerial()
    {
        serial->setPortName(_portName.c_str());
        if (serial->open(QIODevice::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud115200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return true;
        }
        return false;
    }

    //serial
    QSerialPort *serial;
    double _systemOffset; // real_time = time_ardu + _systemOffset
    std::string _portName;
};


#endif //DATACOLLECTOR_ARDUCOMMUNICATOR_H
