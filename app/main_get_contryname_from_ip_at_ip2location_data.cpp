#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <arpa/inet.h>

std::string ipList[]= {
    "127.0.0.1"
};

//
// data format ref https://lite.ip2location.com/database/ip-country
// "0","281470681743359","-","-"
// "281470681743360","281470698520575","-","-"
// "281470698520576","281470698520831","US","United States of America"
//  
//
void printUsage()
{
    std::cerr << "./a.out <cvs file> " << std::endl;
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
    unsigned char buf[sizeof(struct in6_addr)];
    struct in_addr addr;

    if (argc != 2)
    {
        printUsage();
    }
    std::string ipAndCountryListPath = std::string(argv[1]);
    //std::string ip = argv[2];

    //
    // create ip and country pair list 
    std::vector<IpAndCountryInfo> context;
    IpAndCountryInfo::createIpAndCountryInfoList(ipAndCountryListPath, context);

    //
    //
    int len = sizeof(ipList)/sizeof(ipList[0]);
    for(int i=0; i < len; i++) {
        int s = inet_pton(AF_INET, ipList[i].c_str(),&addr);
        std::cout << "addr:" << ipList[i] << "("<< addr.s_addr << ") ";
        std::cout << IpAndCountryInfo::find(context, addr.s_addr).country_name << std::endl;
    }

    return 0;
}
