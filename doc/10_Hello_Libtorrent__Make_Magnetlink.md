
[ Hello Libtorrent : Make Magnetlink]

This document describe how to create a magnet link from torrent file.

Magnet link is a part of URL scheme that begin with "magnet:".

if you already have a torrent client installed on your computer, once you click magnet link, start a torrent client.


# What is Magnetlink

Magnet link 's spcification is bep 0009
https://www.bittorrent.org/beps/bep_0009.html

Magnetlink is defined by bep0009 to share files without using torrent files. 

There is less information in the magnet link than in the torrent file as URLs have length and character limits, magnet link.

magnet link has following information

- info-hash or tagged-info-hash
 Info-hash is metadata's part hash in a torrent file. 

- peer-address
 peer addres is the address of Peer that has the target data
 having Target data torrent client address.
- tr (optional)
 tr is tracker server address.


# Write Code With libtorrent

Next, I explain how to write a code for making magnet link from torrent file with libtorrent.

```
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
```

If you more information magnet link's spec, I recommend to read a libtorrent's code.

 
# Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_tiny_magnetlink_creator.cpp
