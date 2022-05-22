#include<iostream>

#include "CmdPackageInfo.h"

using namespace std;
namespace xsd
{

CmdPackageInfo::CmdPackageInfo():_id(0), _cmd(0)
{

}


CmdPackageInfo::CmdPackageInfo(Package package)
{
    _id = 0;
    for(int i = 0; i < 4; i++)
    {
        _id = (_id<<8) | package[3 - i];
    }
    // std::cout<<hex<<_id<<std::endl;
    // cout<<"package:"<<hex<<package[0] <<" "<<package[1] <<" "<<package[2] <<" "<<package[3] <<std::endl;

    _cmd = 0;
    for(int i = 0; i < 4; i++)
    {
        _cmd = (_cmd<<8) | package[7 - i];
    }
    // std::cout<<hex<<_cmd<<std::endl;
    // cout<<"package:"<<hex<<package[4] <<" "<<package[5] <<" "<<package[6] <<" "<<package[7] <<std::endl;

    for(int i = 8; i < 64; i++)
    {
        _content[i - 8] = package[i];
    }
    // std::cout<<"id:"<<_id<<std::endl;
    // std::cout<<"cmd:"<<_cmd<<std::endl;
}

void CmdPackageInfo::Pack(Package& package)
{
    uint id = _id;
    for(int i = 0; i < 4; i++)
    {
        package[i] = id & 0xff;
        id = id >> 8;
    }

    // std::cout<<hex<<_id<<std::endl;
    // cout<<"Pack package:"<<hex<<package[0] <<" "<<package[1] <<" "<<package[2] <<" "<<package[3] <<std::endl;

    uint cmd = _cmd;
    for(int i = 0; i < 4; i++)
    {
        package[4 + i] = cmd & 0xff;
        cmd = cmd >> 8;
    }
    // std::cout<<hex<<_cmd<<std::endl;
    // cout<<"Pack package:"<<hex<<package[4] <<" "<<package[5] <<" "<<package[6] <<" "<<package[7] <<std::endl;

    for(int i = 8; i < 64; i++)
    {
        package[i] = _content[i - 8];
    }
}

void CmdPackageInfo::Print()
{
    std::cout<<"=======CmdPackageInfo::Print========="<<std::endl;
    std::cout<<"id:"<<_id<<std::endl;
    std::cout<<"cmd:"<<_cmd<<std::endl;
    std::cout<<"content:";
    for(int i =0; i< (int)_content.size(); ++i)
    {
        std::cout<<_content[i];
    }
    std::cout<<std::endl;
    std::cout<<"========CmdPackageInfo::Print End======"<<std::endl;
}
}
