[Hello Libtorrent : Tiny Client]

This document describes how to download a file using libtorrent code.

# Code

First of all, I show you a code to download a data with libtorrent


```
//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_client.cpp -o tiny_client.out -ltorrent-rasterbar -lpthread

#include<iostream>
#include<libtorrent/session.hpp>
#include<libtorrent/alert_types.hpp>
#include<libtorrent/alert.hpp>

void print_usage(std::string);

std::string my_listen_interfaces = "";// "0.0.0.0:6881,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::string target_torrentfile_path = "./a.torrent";

int main(int argc, char *argv[]) try {
	lt::settings_pack session_params;
	session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if(my_listen_interfaces.length() != 0) {
        session_params.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    lt::session session(session_params);

	lt::add_torrent_params torrent_params;// = lt::parse_magnet_uri("<your magnet link>");
	torrent_params.save_path = ".data"; // save in current dir
    torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
	lt::torrent_handle h = session.add_torrent(std::move(torrent_params));


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
```

## Load Torrent File (Meta info)

In the Bittorrent protocol, A Bittorrent client uses a torrenfile to get information　to connection other peer.

A Torrent Client get tracker server address and data informatiom to be downloaded from the torrentfile.


```
lt::add_torrent_params atp;
atp.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
lt::torrent_handle h = session.add_torrent(std::move(atp));
```


## Create Session

In this libtorrent, A Torrent client use a session object to manage p2p resource;

```
lt::settings_pack p;
lt::session session(p);
```

## Add MetaInfo to Session

Once the torrent file information is added to the session object, the session object will start downloading and uploading data.

```
lt::add_torrent_params torrent_params;
torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
lt::torrent_handle h = session.add_torrent(std::move(torrent_params));
```

## Manage Events

A Torrent Client can receive alert info as notification.

First of All, you can set parameters about which event is received.

```
lt::settings_pack session_params;
	session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
lt::session session(session_params);

``` 

And, receive events and display on console.

```
    while (true)
    {
        std::vector<lt::alert *> alerts;
        session.pop_alerts(&alerts);

        lt::state_update_alert *st;
        for (lt::alert *a : alerts)
        {
            std::cout << "[" << a->type() << "](" << a->what() << ") " << a->message() << std::endl;
        }   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
```


## END

You can now download data with libtorrent!!


# REF


## My Memo About Libtorrent And Index 
(0) code-server for libtorrent 2.0 
https://dev.to/kyorohiro/code-server-for-libtorrent-2-0-1m85

## Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_tiny_client.cpp