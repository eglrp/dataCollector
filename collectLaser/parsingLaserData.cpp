//
// Created by jianping on 17-9-9.
//
#include <fstream>
#include "iostream"

int main()
{
    std::ifstream ifs("./laser.dat");
    std::ofstream ofs("pts.xyz");
    while(ifs.good())
    {
        long int sec,usec;
        ifs.read((char*)&sec,sizeof(sec));
        ifs.read((char*)&usec,sizeof(usec));
        int npts;
        ifs.read((char*)&npts,sizeof(npts));
        //std::cout<<sec<<std::endl;
        //std::cout<<usec<<std::endl;
        std::cout<<npts<<std::endl;
        for (int i = 0; i < npts; ++i) {
            double x,y,z;
            int intense;
            unsigned int time;

            ifs.read((char*)(&time),sizeof(time));
            ifs.read((char*)&x,sizeof(x));
            ifs.read((char*)&y,sizeof(y));
            ifs.read((char*)&z,sizeof(z));
            ifs.read((char*)&intense,sizeof(intense));
            ofs<<x<<" "<<y<<" "<<z<<std::endl;
        }
    }
    ofs.close();
    return 0;
}