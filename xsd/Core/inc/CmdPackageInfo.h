#pragma once
#include<string.h>
#include<iostream>
#include "PackageBaseInfo.h"

namespace xsd
{
class  CmdPackageInfo:public PackageBaseInfo
{
    public:
    CmdPackageInfo();
    CmdPackageInfo(Package package);

    void SetId(uint id){ _id = id;}
    uint GetId(){return _id;}

    void SetCmd(uint cmd){ _cmd = cmd;}
    uint GetCmd(){return _cmd;}

    void SetContent(char * content, int size = -1)
    {
        if(size == -1)
        {
            size = strlen(content);
        }
        for(int i = 0; i < size; i++)
        {
            _content[i] = content[i];
        }
    }

    void Pack(Package &package) override;

    void Print();

    bool operator==(const CmdPackageInfo & pkg)
    {
        if(_id != pkg._id)
        {
            std::cout<<"_id:"<<_id<<"pkg.id:"<<pkg._id<<std::endl;
            return false;
        }
        if(_cmd != pkg._cmd)
        {
            std::cout<<"_cmd:"<<_cmd<<"pkg._cmd:"<<pkg._cmd<<std::endl;
            return false;
        }
        
        for(int i = 0; i < (int)pkg._content.max_size(); i++)
        {
            if(_content[i] != pkg._content[i])
            {
                std::cout<<"_content:"<<_content[i]<<"pkg._content:"<< pkg._content[i]<<std::endl;
                return false;
            }
        }
        return true;
    }

    private:
    uint _id;
    uint _cmd;
    std::array<u_char, 56>  _content;
};
}