[Memo : Get Country Name From IP Address At IP2LocationLITE]

We have several way to get geolocation information from ip address.


In this Document, I explain a way to use IP2Location LITE's free data for getting country name from ip.


## Get CVS Format IP List from  IP2LocationLITE 

IP2LocationLITE provides a pair data country and ip.

Get here. 
https://lite.ip2location.com/database/ip-country


## Write a Code at C++

1. Read IP2LocationLITE 's data which is cvs format.
2. Search the country name from ip.
 

```
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

//
// data format ref https://lite.ip2location.com/database/ip-country
// "0","281470681743359","-","-"
// "281470681743360","281470698520575","-","-"
// "281470698520576","281470698520831","US","United States of America"
//  
//
void printUsage()
{
    std::cerr << "./a.out <file path> <ip>" << std::endl;
}

struct IpAndCountryInfo
{
    long begin;
    long end;
    std::string country_name;
    IpAndCountryInfo(std::string line)
    {
        std::stringstream ss;
        ss << line;
        std::string begin_src;
        std::string end_src;

        std::getline(ss, begin_src, ',');
        std::getline(ss, end_src, ',');
        std::getline(ss, country_name, ',');

        if (begin_src.length() > 0 && begin_src[0] == '"')
        {
            begin = std::stol(begin_src.substr(1, begin_src.length() - 2));
        }
        else
        {
            begin = std::stol(begin_src);
        }

        if (end_src.length() > 0 && end_src[0] == '"')
        {
            end = std::stol(end_src.substr(1, end_src.length() - 2));
        }
        else
        {
            end = std::stol(end_src);
        }
    }

    static IpAndCountryInfo find(std::vector<IpAndCountryInfo> ips, long ip)
    {
        int min = 0;
        int max = ips.size() - 1;

        if (ips[min].begin <= ip && ip <= ips[min].end)
        {
            return ips[min];
        }
        if (ips[max].begin <= ip && ip <= ips[max].end)
        {
            return ips[max];
        }
        int index = -1;
        int prev_index = -1;
        do
        {
            prev_index = index;
            index = (max - min) / 2 + min;
            IpAndCountryInfo i = ips[index];
            if (i.begin <= ip && ip <= i.end)
            {
                return i;
            }
            if (ip < i.begin)
            {
                max = index;
            }
            else if (i.end < ip)
            {
                min = index;
            }
            //std::cout << "::" << index<< "::"  << std::endl;
        } while (prev_index != index);

        //for(ipinfo i : ips) {
        //    if(i.begin <= ip && ip<=i.end) {
        //        return i;
        //    }
        //}
        throw std::invalid_argument("ipinfo::find");
    }

    static void createIpAndCountryInfoList(std::string filepath, std::vector<IpAndCountryInfo> &output)
    {
        std::fstream f(filepath);
        std::string l;

        while (std::getline(f, l))
        {
            output.push_back(IpAndCountryInfo(l));
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printUsage();
    }

    std::vector<IpAndCountryInfo> context;
    IpAndCountryInfo::createIpAndCountryInfoList(std::string(argv[1]), context);

    std::cout << IpAndCountryInfo::find(context, 16781311).country_name << std::endl;

    return 0;
}

```


# Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_get_contryname_from_ip_at_ip2location_data.cpp
