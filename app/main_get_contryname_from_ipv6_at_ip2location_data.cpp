#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <arpa/inet.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/asio/ip/address_v6.hpp>

std::string ipList[]= {
    "::1:1",
    "0:0:0:0:0:ffff:1.21.0.0",
    "2c0f:ff20:0:0:0:0:0:0",
    "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"
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
    boost::multiprecision::uint128_t  begin;
     boost::multiprecision::uint128_t  end;
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
            begin_src = begin_src.substr(1, begin_src.length() - 2);
        }
        begin = boost::multiprecision::uint128_t(begin_src);
 
        if (end_src.length() > 0 && end_src[0] == '"')
        {
            end_src = end_src.substr(1, end_src.length() - 2);
        }
        end = boost::multiprecision::uint128_t(end_src);
    }
    static IpAndCountryInfo find(std::vector<IpAndCountryInfo> ips,  boost::multiprecision::uint128_t ip)
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

    static int compare_ipv6(struct in6_addr *ipA, struct in6_addr *ipB)
    {
        //https://stackoverflow.com/questions/37786946/how-to-compare-ipv6-address-in-c-without-using-memcmp
        std::cout << "=== " << std::endl;
        for(int i = 0; i < 16; ++i)
        {
            std::cout<<"v="<<ipA->s6_addr[i] << "," << ipB->s6_addr[i] << std::endl;
            if (ipA->s6_addr[i] < ipB->s6_addr[i]) {
                return -1;
            }
            else if (ipA->s6_addr[i] > ipB->s6_addr[i]){
                return 1;
            }
        }
        return 0;
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
    //

    if (argc != 2)
    {
        printUsage();
    }
    std::string ipAndCountryListPath = std::string(argv[1]);

    //
    // create ip and country pair list 
    std::vector<IpAndCountryInfo> context;
    IpAndCountryInfo::createIpAndCountryInfoList(ipAndCountryListPath, context);

    //
    //
    int len = sizeof(ipList)/sizeof(ipList[0]);

    for(int i=0; i < len; i++) {
        std::cout << "$ " << ipList[i] << " ";
        boost::asio::ip::address_v6 v6 = boost::asio::ip::address_v6::from_string(ipList[i]);
        boost::multiprecision::uint128_t val{};
        for (uint8_t b : v6.to_bytes()) {
            (val <<= 8) |= b;
        }
        std::cout << IpAndCountryInfo::find(context, val).country_name << std::endl;
    }
    return 0;
}
