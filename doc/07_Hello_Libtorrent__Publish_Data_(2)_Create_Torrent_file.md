[Hello Libtorrent : Publish Data (2) Create Torrent file]


In this section, I explain about how to create a file with metadata. These files usually have a torrent extension, like "xxx.torrent".

# What is a torrent file.

These torrent files contain information for connecting other peer and about target data 's HASH and name etc

- Tracker Server Address (Optional) 
- File Name or Directory Name
- Piece Info
- Data's Hash Info
- Data Size
etc


# Create a Torrentfile with Libtorrent

Let's crate a torrent file with libtorrent.


### Source Code

First of all. The completed code is below.

```
//
// Create a torrent file
//  LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_torrentfile_creator.cpp  -o torrentfile_creator.out -ltorrent-rasterbar -lpthread
//
#include<iostream>
#include<vector>
#include<unistd.h>

#include<libtorrent/bencode.hpp>
#include<libtorrent/torrent_info.hpp>
#include<libtorrent/create_torrent.hpp>

std::string to_absolute_path(std::string args);
std::string extract_parent_path(std::string filepath);


int main(int argc, char *argv[]) {
    std::string target_tracker_address = "<Your Tracker Address>";
    std::string target_file_path = "<Your Target File Path>";


    //
    // setup file storage
    std::string source_absolute_path = to_absolute_path(target_file_path);
    lt::file_storage fs;
    lt::create_flags_t flags = {};
    lt::add_files(fs, source_absolute_path, flags);

    // 
    // create torrent creator
    lt::create_torrent torrent(fs, 16*1024);
    if(target_tracker_address.length() > 0) {
        torrent.add_tracker(target_tracker_address);
    }
    //
    // add hash info
    lt::set_piece_hashes(torrent, extract_parent_path(source_absolute_path));

    //
    // generate metadata
    lt::entry entry_info = torrent.generate();
    
    //
    // convert to bencoding
    std::vector<char> torrentfile_source;
    lt::bencode(std::back_inserter(torrentfile_source), entry_info); // metainfo -> binary
    std::cout.write(torrentfile_source.data(),torrentfile_source.size()); // binary -> stdout
    
    return 0;
}

std::string to_absolute_path(std::string path) {
    if(path.length() == 0) {
        throw std::runtime_error("failed to current_dir");
    }
    if(path[0] == '/') {
        return path;
    }
    char current_dir_path[2056];
    auto ret = getcwd(current_dir_path, sizeof(current_dir_path));
    if(ret == NULL) {
        std::cerr << "failed to current_dir ";
        throw std::runtime_error("failed to current_dir");
    }
    if(0==path.compare(0, 2, "./")) {
        path = path.substr(2);
    }
    return std::string(current_dir_path) + "/" + path;
} 

std::string extract_parent_path(std::string filepath)
{
  if (filepath.empty()) {
    return filepath;
  }
  int pos = filepath.find_last_of("/");
  
  return filepath.substr(0,pos);
}

```


### Setup target file info

Torrent files can contain multiple file information.
These structures can be described using file storage objects.

like this.

```
std::string source_absolute_path = to_absolute_path(target_file_path);
lt::file_storage fs;
lt::create_flags_t flags = {};
lt::add_files(fs, source_absolute_path, flags);
```

### Setup Torrentfile Info

Torrent file contain tracker address and piece length and target file info.

like this.

```
lt::create_torrent torrent(fs, 16*1024);
torrent.add_tracker(target_tracker_address);
lt::set_piece_hashes(torrent, extract_parent_path(source_absolute_path));
lt::set_piece_hashes(torrent, extract_parent_path(source_absolute_path));
```


### Generate binary data

Torrent files are written in bencode format.

like this.

```
lt::entry entry_info = torrent.generate();
std::vector<char> torrentfile_source;
lt::bencode(std::back_inserter(torrentfile_source), entry_info); // metainfo -> binary  
std::cout.write(torrentfile_source.data(),torrentfile_source.size()); // binary -> stdout 

```

Good!! You can now create a torrent file.

### Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_tiny_torrentfile_creator.cpp


