//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_log_analysis.cpp -o tiny_log_analysis.out -ltorrent-rasterbar -lpthread

#include<iostream>
#include<libtorrent/session.hpp>
#include<libtorrent/alert_types.hpp>
#include<libtorrent/alert.hpp>
#include<libtorrent/magnet_uri.hpp>
#include<libtorrent/error_code.hpp>

//
#include<regex>
#include<sstream>

//
// Prpoperty
//
std::string my_listen_interfaces = "";// "0.0.0.0:6881,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::vector<std::string> target_torrentfiles  = {
    "./a.torrent"
};//"../Big_Buck_Bunny_1080p_surround_frostclick.com_frostwire.com.torrent"};
std::vector<std::string> target_magnetlinks  = {};



std::map<std::string, std::vector<std::string>> ip_list_map ={};
std::regex re("(.*):.*add_peer\\(\\).*\\[(.*)\\].*");

void extract_ip_list_from_log(std::string log);
void display_analysis_result();

const int upload_max = -1; // 0 is unsetting no limit
const int download_max = -1; // 0 is unsetting nolimit


void print_usage(std::string);



int main(int argc, char *argv[]) try {

    //
    // create session
    //
    lt::settings_pack session_params;
    session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if (my_listen_interfaces.length() != 0)
    {
        session_params.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    //session_params.set_bool(lt::settings_pack::enable_dht, false);
    lt::session session(session_params);

    //
    // torrent param
    //

    // add torrent
    for(std::string t : target_torrentfiles) {
        lt::add_torrent_params torrent_params;
        torrent_params.save_path = ".data"; // save in this dir
        torrent_params.ti = std::make_shared<lt::torrent_info>(t);
        lt::torrent_handle h = session.add_torrent(std::move(torrent_params));
        if(upload_max >= 0 ) {
            h.set_upload_limit(upload_max);
        }
        if(download_max >= 0) {
            h.set_download_limit(download_max);
        }
    }

    // add magnet link
    for(std::string m :target_magnetlinks) {
        lt::add_torrent_params torrent_params;
        torrent_params.save_path = ".data"; // save in this dir
        lt::error_code ec;
		lt::parse_magnet_uri(m,torrent_params, ec);
		if (ec){
            std::cerr << "wrong magnet link "  << m << std::endl;
            throw std::invalid_argument("wrong magnet link \""+ m +"\"");
        }
        torrent_params = lt::parse_magnet_uri(m);
        lt::torrent_handle h = session.add_torrent(torrent_params);
        if(upload_max >= 0 ) {
            h.set_upload_limit(upload_max);
        }
        if(download_max >= 0) {
            h.set_download_limit(download_max);
        }
    }


    //
    // Log Analysis
    //
    while (true)
    {
        std::vector<lt::alert *> alerts;
        session.pop_alerts(&alerts);

        lt::state_update_alert *st;
        for (lt::alert *a : alerts)
        {
            std::cout << "[" << a->type() << "](" << a->what() << ") " << a->message() << std::endl;
            extract_ip_list_from_log(a->message());
            switch (a->type())
            {
            case lt::torrent_finished_alert::alert_type:
                std::cout << ">> finished : ==" << std::endl;
                goto END;
            case lt::torrent_error_alert::alert_type:
                std::cout << ">> error : ==" << std::endl;
                goto END;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
END:
    display_analysis_result();
    return 0;
}
catch (std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}

void extract_ip_list_from_log(std::string log) {
    std::smatch matches;
    if(std::regex_match(log, matches, re) && matches.size() == 3){
        std::string key = matches[1];
        std::string value = matches[2];
        //std::cout << "Matched" << matches[1] << ":" << matches[2]<<std::endl;        
        if(ip_list_map.count(key) <= 0) {
            ip_list_map[key] = std::vector<std::string>{};
        }
        
        std::vector<std::string> &ref = ip_list_map[key];
        //not contain
        std::istringstream f(value);
        std::string s;
        while(std::getline(f, s, ' ')) {          
            if(std::find(ref.begin(), ref.end(), s) == ref.end()) {
                ip_list_map[key].push_back(s);
            }
        }
    }
}

void display_analysis_result() {
    for(auto const& i: ip_list_map){
        std::cout << "[" << i.first << "]" << std::endl;      
        for(auto const& j: i.second ) {
            std::cout << " - " << j << "" << std::endl;
        }
    }
}
