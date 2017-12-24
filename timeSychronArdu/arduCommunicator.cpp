//
// Created by jianping on 17-12-20.
//

#include "arduCommunicator.h"
#include "sys/time.h"
#include "QDebug"

ArduCommunicator::ArduCommunicator() {
    serial = new QSerialPort();
}

QByteArray ArduCommunicator::readData() {
    serial->waitForReadyRead(50);
    QByteArray raw_data = serial->readAll();
    return raw_data;
}

bool ArduCommunicator::getTime() {

    std::vector<double> offsets;
    for (int i = 0; i < 10; ++i) {
        timeval tv;
        gettimeofday(&tv,NULL);
        serial->write(QByteArray("g"));
        QByteArray t_data = readData();
        unsigned int count=0; unsigned int time_ardu=0;
        memcpy((char*)&count,&t_data.data()[0],4);
        memcpy((char*)&time_ardu,&t_data.data()[4],4);
        offsets.push_back(tv.tv_sec+(tv.tv_usec-time_ardu)/1000000.0);
        usleep(100000);
    }
    double average = 0;
    for (int j = 0; j < offsets.size(); ++j) {
        //qDebug("offset:%lf",offsets[j]);
        average = average + (offsets[j]-offsets[0]);
    }
    average = average/offsets.size() + offsets[0];
    //qDebug("average offset:%lf",average);
    _systemOffset = average;
    qDebug("Arduino average offset:%lf",average);
}

double ArduCommunicator::readMeasurement() {

    while (serial->bytesAvailable() < 8){
        serial->waitForReadyRead(50);
    }

    unsigned char c;
    serial->read((char*)&c,1);
    if(c != 0xFF)//head of data
    {
        qDebug("1:%d",c);
        return -1;
    }
    serial->read((char*)&c,1);
    if(c != 0x00)//head of data
    {
        qDebug("2:%d",c);
        return -1;
    }
    unsigned char data[4];
    serial->read((char*)&data[0],4);
    serial->read((char*)&c,1);//check sum
    unsigned char sum = 0xFF + data[0] + data[1] + data[2] + data[3];
    if((sum+c)&0xFF != 0xFF)
    {
        qDebug("check sum is not OxFF: %d",sum+c);
        return -1;
    }

    serial->read((char*)&c,1);
    if(c != 0x00)// end of data
    {
        return -1;
    }

    unsigned int ardu_time = 0;
    memcpy((char*)&ardu_time,(char*)&data[0],4);
    //qDebug("arduino time : %d", ardu_time);

    return _systemOffset + ardu_time/1000000.0;
}

bool ArduCommunicator::startMeasure(std::ofstream &ofs) {
    serial->write(QByteArray("m"));
    int count = 0;
    while(1)
    {
        double time = readMeasurement();
        if(time == -1)
        {
            qDebug("fail to read");
        } else{
            //qDebug("time: %lf",time);
            long sec = 0;
            long usec = 0;
            sec = floor(time);
            usec = (time-sec)*1000000;
            char buf[64];
            sprintf(buf,"%010ld,%08ld\n",sec,usec);
            ofs<<buf;
        }
        count++;
        if(count >= 5)
        {
            ofs.flush();
            count = 0;
        }
    }

}