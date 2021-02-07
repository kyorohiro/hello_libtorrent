[Hello Libtorrent : Publish Data (3) Start Seeder]

I had explained about how to start tracker server and how to create a meta data file.

In this section, I explain how to create seeder with libtorrent.

# What is Seeder
Seeder is a peer uploading data. Seeder has many task.

- Regist own ip address and own status to tracker server
- Join the mainline dht network
- Explore peers which want to data 
- Download and Upload Game
- Upload Data

etc..

But,You can write so easy a seeder code  with  using libtorrent



# Source Code 

It's a completed code is below.
 
```
//LD_LIBRARY_PATH=/usr/local/lib/libtorrent-rasterbar.so g++ main_tiny_seeder.cpp -o tiny_seeder.out -ltorrent-rasterbar -lpthread

#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/alert.hpp>


std::string my_listen_interfaces = "";             //<your ip>:6881";//,[::]:6881"  "{E4F0B674-0DFC-48BB-98A5-2AA730BDB6D6}:7777"
std::string target_torrentfile_path = "a.torrent"; // <your torrent file  path>
std::string target_folder_path = ".data";

int main(int argc, char *argv[])
try
{
    lt::session session();
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

It's so Simple Code.

## Create Session

Session object manage all state about downloading and uploading and exploring
 
```
lt::session session();
```

## Add Torrent to Session Object

Once the torrent file information is added to the session object, the session object will start uploading, downloading, and exploring.

```
lt::add_torrent_params torrent_params; // = lt::parse_magnet_uri("<your magnet link>");
    torrent_params.save_path = ".data";    // save in current dir
torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
ssession.add_torrent(std::move(torrent_params));
```


## Output Status

Session Object can notify alert any messages.

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


# Optional

If you has known ip address, you can set this

```
lt::settings_pack session_params;
session_params.set_int(lt::settings_pack::alert_mask, lt::alert_category::all);
    if (my_listen_interfaces.length() != 0)
    {
        session_params.set_str(lt::settings_pack::listen_interfaces, my_listen_interfaces);
    }
    lt::session session(session_params);
```

and libtorrent has seeder mode.

```
lt::add_torrent_params torrent_params; // = lt::parse_magnet_uri("<your magnet link>");
torrent_params.save_path = ".data";    // save in current dir
torrent_params.ti = std::make_shared<lt::torrent_info>(target_torrentfile_path);
torrent_params.flags |= lt::torrent_flags::seed_mode; // May not be needed
torrent_params.have_pieces.set_all();                 //　May not be needed
torrent_params.verified_pieces.set_all();             // May not be needed
```

and has super seeder mode too.
 
## All Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_tiny_seeder.cpp

