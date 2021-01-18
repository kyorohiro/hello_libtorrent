//
// Create a torrent file
//  LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_torrentfile_creator.cpp -o creator.out -ltorrent-rasterbar -lpthread
#include<iostream>
#include<vector>
#include<unistd.h>

#include<libtorrent/bencode.hpp>
#include<libtorrent/torrent_info.hpp>
#include<libtorrent/create_torrent.hpp>

std::string to_absolute_path(std::string args);
std::string extract_parent_path(std::string filepath);
void print_usage(std::string execute_file_name);

int main(int argc, char *argv[]) {
    std::string tracker_url = "";
    std::string source_path = "";

    // chars to strings 
    std::vector<std::string> args;
    for(int i=1; i<argc;i++) {
        args.push_back(std::string(argv[i]));
    }

    if(args.size() <= 0) {
        print_usage(std::string(argv[0]));
        return -1;
    }

    for(auto v=args.begin(); v != args.end();v++){
        if(v->length() > 0 && v->rfind("-", 0) == 0){
            if((v+1) == args.end()){
                break;
            }
        }
        if(std::string("-a") == *v) {
            tracker_url = *(++v);
            continue; 
        }
        else {
            source_path = *v;
        }
    }

    std::cerr << "  tracker url = " << tracker_url<< std::endl;
    std::cerr << "  absolute path = " << to_absolute_path(source_path)<< std::endl;
    std::cerr << "  parent path = " << extract_parent_path(to_absolute_path(source_path))<< std::endl;

    std::string source_absolute_path = to_absolute_path(source_path);
    lt::file_storage fs;
    lt::create_flags_t flags = {};
    lt::add_files(fs, source_absolute_path, flags);
    lt::create_torrent torrent(fs, 16*1024);
    if(tracker_url.length() > 0) {
        torrent.add_tracker(tracker_url);
    }
    lt::set_piece_hashes(torrent, extract_parent_path(source_absolute_path));
    lt::entry entry_info = torrent.generate();
    
    std::vector<char> torrentfile_source;
    lt::bencode(std::back_inserter(torrentfile_source), entry_info);
    std::cout.write(torrentfile_source.data(),torrentfile_source.size());
    
    return 0;
}

void print_usage(std::string args0) {
    std::cerr << ""<< args0 << " <filename> [ -a <tracker address> ]" << std::endl;
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