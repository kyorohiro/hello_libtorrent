[Hello Libtorrent : Bencode]

Hi, I'm exploring  how to use libtorrent now . 
And This is memo about that. 


# What is Bencode

bencode is a data format. bencode support 4 data type.

- number
- string and binary
- list
- dict

Bencode spec is defined at http://bittorrent.org/beps/bep_0003.html


### Number
Number 123 is described as follows

```
i123e
```

### String
String hello is described as follows

```
5:hello
```

### List

List has hello and 12 which is described as follows

```
l5:helloi12ee
```

### Dict 
A dict is stored  a element which conbination of "num" key and 12 value.   

```
d3:numi12ee
```


# How to write a code for becode

```
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
```


```
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
```

# Source Code
https://github.com/kyorohiro/hello_libtorrent

# Ref

http://bittorrent.org/beps/bep_0003.html

https://blog.libtorrent.org/2015/03/bdecode-parsers/


# My Memo About Libtorrent And Index 
(0) code-server for libtorrent 2.0 
https://dev.to/kyorohiro/code-server-for-libtorrent-2-0-1m85