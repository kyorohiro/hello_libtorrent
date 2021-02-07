[code-server for libtorrent 2.0 ]

Libtorrent is version 2.0 rc now!!
It Great!!
https://github.com/arvidn/libtorrent


I created a develop environment at docker. In order to give it  a try.

https://github.com/kyorohiro/libtorrent_codeserver


# Unique Point
This Develop Environment's Good Point

- bundled vscode 
- using docker
- autocomplete at vscode c++plugin 

You can develop a torrent client with libtorrent Immediately!!
This Pacakge Has Already installed C++ and vscode c++ plugin!!


# Procedure

## Prepare

```
git clone https://github.com/kyorohiro/libtorrent_codeserver
cd libtorrent_codeserver
```

```
cd res 
git clone https://github.com/arvidn/libtorrent
cd libtorrent
git checkout RC_2_0
git submodule init 
git submodule update
```

## RUN  Docke Image

```
docker-compose build
docker-compose up -d
```

## Open VSCODE on Browser

open  "http://127.0.0.1:8443/"
※ 127.0.0.1 is your docker ip

## BUILD HELLO WORLD

```
g++ main_simple_torrent.cpp  -ltorrent-rasterbar -lpthread
```

## START A CLIENT

```
./a.out xxx.torrent
```


