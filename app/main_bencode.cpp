//
//  LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_bencode.cpp -ltorrent-rasterbar -lpthread
//
#include<iostream>
#include<libtorrent/bdecode.hpp>
#include<libtorrent/bencode.hpp>
#include<vector>
#include<stdio.h>



// [Memo]
// using dictionary_type = std::map<std::string, entry, aux::strview_less>;
// using string_type = std::string;
// using list_type = std::vector<entry>;
// using integer_type = std::int64_t;
// using preformatted_type = std::vector<char>;

int main() {
    //
    // encode
    //
    {
        // string 
        std::string s;
        lt::bencode(std::back_inserter(s), lt::entry("hello"));
        std::cout << s << std::endl; // 5:hello
    }
    {
        // number
        std::string s;
        lt::bencode(std::back_inserter(s), lt::entry(12));
        std::cout << s << std::endl; // i12e
    }
    {
        // list
        std::string s;
        lt::bencode(std::back_inserter(s), lt::entry(std::vector<lt::entry>{
            lt::entry("hello"),lt::entry(12)
        }));
        std::cout << s << std::endl; // l5:helloi12ee
    }
    {
        // dict
        std::string s;
        lt::bencode(std::back_inserter(s), lt::entry(
           lt::entry::dictionary_type {
                {"num", lt::entry(12)},
                {"str", lt::entry("hello")}
            }
        ));
        std::cout << s << std::endl; // d3:numi12e3:str5:helloe
    }
    {
        // binary
        std::string s;
        lt::bencode(std::back_inserter(s), lt::entry(std::vector<char>{(char)0xFF,(char)0x01,(char)0x03}));
        std::cout << s << std::endl; // i-12e
    }
    {
        // other
        std::vector<char> output;
        lt::entry source_data;

        source_data["bstring"] = lt::entry::string_type("hello");
        source_data["bint"] = lt::entry::integer_type(12);
        source_data["elist"] = std::vector<lt::entry>{
            lt::entry(lt::entry::string_type("01")), lt::entry(lt::entry::integer_type(1))
        };
        source_data["tperformatted_type"] = lt::entry::preformatted_type{(char)0xFF,(char)0x01,(char)0x03};

        lt::bencode(std::back_inserter(output), source_data);
        std::cout << std::string(output.data(), output.size()) << std::endl;
    }

    return 0;
}