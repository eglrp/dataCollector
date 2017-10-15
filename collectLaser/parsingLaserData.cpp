//
// Created by jianping on 17-9-9.
//
#include <fstream>
#include "iostream"

int main()
{
    std::string filename;
    std::cin>>filename;
    std::ifstream ifs(filename);
    std::ofstream ofs("pts.xyz");
    int count = 0;
    while(ifs.good())
    {
        long int sec,usec;
        ifs.read((char*)&sec,sizeof(sec));
        ifs.read((char*)&usec,sizeof(usec));
        int npts;
        ifs.read((char*)&npts,sizeof(npts));
        //std::cout<<sec<<std::endl;
        //std::cout<<usec<<std::endl;
        //ofs<<"start\n"<<npts<<std::endl;
        std::cout<<npts<<std::endl;

        for (int i = 0; i < npts; ++i) {
            double x,y,z;
            unsigned char intense;
            unsigned int time;

            ifs.read((char*)(&time),sizeof(time));
            ifs.read((char*)&x,sizeof(x));
            ifs.read((char*)&y,sizeof(y));
            ifs.read((char*)&z,sizeof(z));
            ifs.read((char*)&intense,sizeof(intense));
            if(count >= 1000 && count <= 1200)
            {
                ofs<<x<<" "<<y<<" "<<z<<std::endl;
            }

            //std::cout<<x<<" "<<y<<" "<<z<<std::endl;
        }
        count ++;
    }
    ofs.close();
    return 0;
}