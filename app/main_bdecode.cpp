//
//  LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_bdecode.cpp -ltorrent-rasterbar -lpthread
//
#include<iostream>
#include<libtorrent/bencode.hpp>
#include<libtorrent/bdecode.hpp>

int main(int argc, char* argv[]) {
    {
        // deprecated? by TORRENT_DEPRECATED
        lt::entry result = lt::bdecode(std::string("i12e"));
        std::cout << result.integer() << std::endl; // 12
    }

    {
        // string 
        std::cout << "# string" << std::endl; 
        lt::error_code ec;
        lt::bdecode_node e = lt::bdecode("5:hello", ec);
        std::cout << e.string_value() << std::endl; 
    }
    {
        // number
        std::cout << "# number" << std::endl; 
        lt::error_code ec;
        lt::bdecode_node e = lt::bdecode("i12e", ec);
        std::cout << e.int_value() << std::endl; 
    }
    {
        // list
        std::cout << "# list" << std::endl; 
        lt::error_code ec;
        lt::bdecode_node e = lt::bdecode("l5:helloi12ee", ec);
        
        std::cout << e.list_at(0).string_value() << std::endl; 
        std::cout << e.list_at(1).int_value() << std::endl; 
    }
    {
        // dict
        std::cout << "# dict" << std::endl; 
        lt::error_code ec;
        lt::bdecode_node e = lt::bdecode("d3:numi12e3:str5:helloe", ec);
        
        std::cout << e.dict_find("num").int_value() << std::endl; 
        std::cout << e.dict_find("str").string_value() << std::endl;
    }
    return 0;
}