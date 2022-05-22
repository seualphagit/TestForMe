#pragma once

#include<string>
#include<array>

namespace xsd
{
using uchar = unsigned char;
using uint = unsigned int;
using Package = std::array<uchar, 64>;

class PackageBaseInfo
{
    public:

    virtual void Pack(std::array<uchar, 64> &package)=0;
};

}