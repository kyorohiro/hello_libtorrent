[Hello Libtorrent : Extension Torrent Plugin]

In this section I will explain how to get the IP address of the peer.

I explained how to get peer's IP address to use alert notification 's message. 

But if you use the libtorrent extension, you can do that 

# This time's complete Code

```
#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/error_code.hpp>


// ---
struct test_plugin_torrent : lt::torrent_plugin
{
    void on_add_peer(lt::tcp::endpoint const &endpoint, lt::peer_source_flags_t flag1, lt::add_peer_flags_t flag2) override
    {
        std::cout << "[on_add_peer]" << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;
    }
    void on_state(lt::torrent_status::state_t st)
    {
        std::cout << "[on_state]" << st << std::endl;
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

```

# Torrent Plugin
by using Torrent plugin. you can get more detailed informatons.

you can hook any event with torrent_plugin 
- virtual void on_piece_pass(piece_index_t) {}
- virtual void on_piece_failed(piece_index_t) {}
- virtual bool on_pause() { return false; }
- virtual bool on_resume() { return false; }
- virtual void on_files_checked() {}
- virtual void on_state(torrent_status::state_t) {}
- virtual void on_add_peer(tcp::endpoint const&....


# Get Peer Address

In this sample code, I hooked the on_add_peer event.
You can get the peer address from the on_add_peer event information. 

You can hook it by registering the plugin object in the torrent_param object.

```
torrent_params.extensions.push_back(creator);
```

```
struct test_plugin_torrent : lt::torrent_plugin
{
    void on_add_peer(lt::tcp::endpoint const &endpoint, lt::peer_source_flags_t flag1, lt::add_peer_flags_t flag2) override
    {
        std::cout << "[on_add_peer]" << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;
    }
};
```

And Get There

````
[on_add_peer]xx.xx.xx.x:7881
[on_add_peer]yy.y.y.y.y:37151
[on_add_peer]zz.zz.z.z.z:6881
[on_add_peer]cc.cc.cc.20:6881
[on_add_peer]84.aaa.ddd.152:58515
...
````

END.
# Source

https://github.com/kyorohiro/hello_libtorrent
app/main_extension_torrent_plugin.cpp
