//
// export LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so.2.0.2
// g++ main_extension_peer_plugin.cpp -o main_extension_peer_plugin.out -ltorrent-rasterbar -lpthread
//

#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/peer_connection_handle.hpp>

// ---
struct test_peer_plugin : lt::peer_plugin
{
    std::string _id = "";
    test_peer_plugin(std::string id)
    {
        _id = id;
    }

    void add_handshake(lt::entry &e) override
    {
        std::cout << _id << " add_handshak " << e.to_string() << std::endl;
    }
    bool on_have(lt::piece_index_t i) override
    {
        std::cout << _id << " on_have " << i << std::endl;
        return false;
    }
    bool on_dont_have(lt::piece_index_t i) override
    {
        std::cout << _id << " on_dont_have " << i << std::endl;
        return false;
    }
    bool on_bitfield(lt::bitfield const &bitfield) override
    {
        std::cout << _id << " on_bitfield " << bitfield.count() << "/" << bitfield.size() << std::endl;
        return false;
    }
    bool on_have_all() override
    {
        std::cout << _id << " on_have_all " << std::endl;
        return false;
    }
    bool on_have_none() override
    {
        std::cout << _id << " on_have_none " << std::endl;
        return false;
    }
    void on_disconnect(lt::error_code const &e) override
    {
        std::cout << _id << " on_disconnect " << e.message() << std::endl;
    }

    void on_connected() override
    {
        std::cout << _id << " on_connect " << std::endl;
    }
};
struct test_plugin_torrent : lt::torrent_plugin
{
    std::shared_ptr<lt::peer_plugin> new_connection(lt::peer_connection_handle const &pch) override
    {
        std::stringstream ss;
        ss << "[remote_ip]=" << pch.remote().address().to_string() << ":" << pch.remote().port();
        ss << "[local_ip]=" << pch.local_endpoint().address().to_string() << ":" << pch.local_endpoint().port();
        std::cout << " new connection 1 " << ss.str() << std::endl;
        return std::make_shared<test_peer_plugin>(ss.str());
    }

    void on_add_peer(lt::tcp::endpoint const &endpoint, lt::peer_source_flags_t flag1, lt::add_peer_flags_t flag2) override
    {
        //using peer_source_flags_t = flags::bitfield_flag<std::uint8_t, struct peer_source_flags_tag>;
        //using alert_category_t    = flags::bitfield_flag<std::uint32_t, struct alert_category_tag>;
        std::string type = "other";
        if (lt::peer_info::dht == flag1)
        {
            type = "dht";
        }
        else if (lt::peer_info::tracker == flag1)
        {
            type = "tracker";
        }
        else if (lt::peer_info::lsd == flag1)
        {
            type = "lsd";
        }
        std::cout << "[on_add_peer][" << type << "]" << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;
    }
};

struct plugin_creator
{
    std::shared_ptr<lt::torrent_plugin>
    operator()(lt::torrent_handle const &, lt::client_data_t)
    {
        return std::make_shared<test_plugin_torrent>();
    }
};
// ----

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
    plugin_creator creator;

    // ----
    torrent_params.extensions.push_back(creator);
    // ----

    lt::torrent_handle h = session.add_torrent(std::move(torrent_params));

    session.add_torrent(torrent_params);

    while (true)
    {
        std::vector<lt::alert *> alerts;
        session.pop_alerts(&alerts);

        lt::state_update_alert *st;
        for (lt::alert *a : alerts)
        {
            //std::cout << "[" << a->type() << "](" << a->what() << ") " << a->message() << std::endl;
            switch (a->type())
            {
                //case lt::torrent_finished_alert::alert_type:
                //   goto END;
                //case lt::torrent_error_alert::alert_type:
                //    goto END;
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
