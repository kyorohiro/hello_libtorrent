//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_magnetlink_creator.cpp -o tiny_magnetlink_creator.out -ltorrent-rasterbar -lpthread

#include<iostream>
#include<libtorrent/magnet_uri.hpp>

void print_usage(std::string args0) {
    std::cerr << ""<< args0 << " <torrentfile path>" << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        print_usage(argv[0]);
        return -1;
    }
    lt::error_code ec;
    lt::torrent_info torrent_info(std::string(argv[1]), ec);

    std::cout << lt::make_magnet_uri(torrent_info);
    return 0;
}