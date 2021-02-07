[Hello Libtorrent : Extension Peer Plugin]

Last time, I introduced the torrent plugin. This time I will introduce the Peer Plugin.

By using Peer Plugin, you can monitor the interaction between Peers. 

Various communications are taking place between peers.

For Example Seeder is 
- Explorer Peer with Tracker and DHT
- Connect Founded Peer
- Shakehand between peers
- Send Bitfiled Format Data About Piece Info 
- Receive Request 
- Response This Request
- ..
- ..

You can hook these events by using the Peer Plugin. 

```
struct TORRENT_EXPORT peer_plugin
{
    virtual void add_handshake(...
    virtual void on_disconnect(...
    virtual void on_connected(...
    virtual bool on_handshake(...
    virtual bool on_extension_handshake(...
    virtual bool on_choke(...
    virtual bool on_unchoke(...
    virtual bool on_interested(...
    virtual bool on_not_interested(...
    virtual bool on_have(...
    virtual bool on_dont_have(...
    virtual bool on_bitfield(...
    virtual bool on_have_all(...
    virtual bool on_have_none(...
    virtual bool on_allowed_fast(...
    virtual bool on_request(...
    virtual bool on_piece(...
    virtual bool on_cancel(...
    virtual bool on_reject(...
    virtual bool on_suggest(...
    virtual void sent_have_all(...
    virtual void sent_have_none(...
    virtual void sent_reject_request(...
    virtual void sent_allow_fast(...
    virtual void sent_suggest(...
    virtual void sent_cancel(...
    virtual void sent_request(...
    virtual void sent_choke(...
    virtual void sent_unchoke(...
    virtual void sent_interested(...
    virtual void sent_not_interested(...
    virtual void sent_have(...
    virtual void sent_piece(...
    virtual void sent_payload(...
    virtual bool can_disconnect(...
    virtual bool on_extended(...
    virtual bool on_unknown_message(...
    virtual void on_piece_pass(...
    virtual void on_piece_failed(...
    virtual void tick(...
    virtual bool write_request(...
};
``` 

# Sample Code 

```
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
``` 


It looks like a long code, but I've only added the following from the last time.


#### Write Your Peer Plugin 


You can create  your  peer plugin by extending the peer_plugin object.


```
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
```

#### Regist Peer Plugin

You can regist peer plugin from torrent plugin 's new_connection method.
 
```

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
..
}
```



# Source

https://github.com/kyorohiro/hello_libtorrent
app/main_extension_peer_plugin.cpp


