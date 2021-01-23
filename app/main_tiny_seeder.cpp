//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_seeder.cpp -o tiny_seeder.out -ltorrent-rasterbar -lpthread

#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/alert.hpp>

void print_usage(std::string);

std::string my_listen_interfaces = "";             //<your ip>:6881";//,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::string target_torrentfile_path = "a.torrent"; // <your torrent file  path>
std::string target_folder_path = ".data";

int main(int argc, char *argv[])
try
{
    lt::settings_pack session_params;
    session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if (my_listen_interfaces.length() != 0)
    {
        session_params.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    lt::session session(session_params);

    lt::add_torrent_params torrent_params; // = lt::parse_magnet_uri("<your magnet link>");
    torrent_params.save_path = ".data";    // save in current dir
    torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
    torrent_params.flags |= lt::torrent_flags::seed_mode; // May not be needed
    torrent_params.have_pieces.set_all();                 //　May not be needed
    torrent_params.verified_pieces.set_all();             // May not be needed

    lt::torrent_handle h = session.add_torrent(std::move(torrent_params));

    while (true)
    {
        std::vector<lt::alert *> alerts;
        session.pop_alerts(&alerts);

        lt::state_update_alert *st;
        for (lt::alert *a : alerts)
        {
            std::cout << "[" << a->type() << "](" << a->what() << ") " << a->message() << std::endl;
            /*switch (a->type())
            {
            case lt::torrent_finished_alert::alert_type:
                std::cout << ">> finished : ==" << std::endl;
                goto END;
            case lt::torrent_error_alert::alert_type:
                std::cout << ">> error : ==" << std::endl;
                goto END;
            }*/
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
END:
    return 0;
}
catch (std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}