//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_client.cpp -o tiny_client.out -ltorrent-rasterbar -lpthread

#include<iostream>
#include<libtorrent/session.hpp>
#include<libtorrent/alert_types.hpp>
#include<libtorrent/alert.hpp>

void print_usage(std::string);

std::string my_listen_interfaces = "";// "0.0.0.0:6881,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::string target_torrentfile_path = "./a.torrent";

int main(int argc, char *argv[]) try {
	lt::settings_pack p;
	p.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if(my_listen_interfaces.length() != 0) {
        p.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    lt::session session(p);

	lt::add_torrent_params atp;// = lt::parse_magnet_uri("<your magnet link>");
	atp.save_path = ".data"; // save in current dir
    atp.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
	lt::torrent_handle h = session.add_torrent(std::move(atp));


    while (true)
    {
        std::vector<lt::alert *> alerts;
        session.pop_alerts(&alerts);

        lt::state_update_alert *st;
        for (lt::alert *a : alerts)
        {
            std::cout << "[" << a->type() << "](" << a->what() << ") " << a->message() << std::endl;
            switch (a->type())
            {
            case lt::state_update_alert::alert_type:
                //lt::state_update_alert
                st = (lt::state_update_alert *)(a);
                {
                    lt::torrent_status const &s = st->status[0];
                    std::cout << '\r' //<< lt::state(s.state) << ' '
                              << (s.download_payload_rate / 1000) << " kB/s "
                              << (s.total_done / 1000) << " kB ("
                              << (s.progress_ppm / 10000) << "%) downloaded ("
                              << s.num_peers << " peers)\x1b[K";
                    std::cout.flush();
                }
                break;
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
    return 0;
}
catch (std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}