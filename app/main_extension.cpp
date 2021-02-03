//
// export LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so
// g++ main_extension.cpp -o main_extension.out -ltorrent-rasterbar -lpthread
//

#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/error_code.hpp>


struct test_plugin_torrent : lt::torrent_plugin {
    // the flags indicating which sources a peer can
	// have come from. A peer may have been seen from
	// multiple sources
	// using peer_source_flags_t = flags::bitfield_flag<std::uint8_t, struct peer_source_flags_tag>;
    // using add_peer_flags_t    = flags::bitfield_flag<std::uint8_t, struct add_peer_flags_tag>;
    void on_add_peer(lt::tcp::endpoint const & endpoint, lt::peer_source_flags_t flag1, lt::add_peer_flags_t flag2) override
    {
        std::cout << "[on_add_peer]" << endpoint.address().to_string() <<":" << endpoint.port() << std::endl;
    }
    void on_state(lt::torrent_status::state_t st) {
       std::cout << "[on_state]" << st << std::endl;      
    }
};

struct plugin_creator
{
	explicit plugin_creator(int& c) : m_called(c) {}

	std::shared_ptr<lt::torrent_plugin>
	operator()(lt::torrent_handle const&, lt::client_data_t)
	{
		return std::make_shared<test_plugin_torrent>();
	}
	int& m_called;
};

std::string my_listen_interfaces = ""; // "0.0.0.0:6881,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::string target_torrentfile_path = "a.torrent";

int main(int argc, char *argv[])
try
{

    //
    // createsession 
    lt::settings_pack session_params;
    session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if (my_listen_interfaces.length() != 0)
    {
        session_params.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    lt::session session(session_params);

    //
    // add torrent
    lt::add_torrent_params torrent_params;
    torrent_params.save_path = ".data"; // save in this dir
    torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
	int called = 0;
	plugin_creator creator(called);
    torrent_params.extensions.push_back(creator);
    lt::torrent_handle h = session.add_torrent(std::move(torrent_params));

    session.add_torrent(torrent_params);
    
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
            case lt::torrent_finished_alert::alert_type:
                goto END;
            case lt::torrent_error_alert::alert_type:
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
