[Hello Libtorrent : Publish Data (1) Start Your Tracker Server ]

This document is a tutorial to publish a data with your opentracker and your libtorrent client. 


# Start Your Tracker Server
Preparing Tracker Your Server is not always necessary. It is optionnal now.  Because Bittorrent support trackerless protocol.

But, This time, In this Document, You prepare your tracker server. 

It is a basica way according to a http://bittorrent.org/beps/bep_0052.html and http://bittorrent.org/beps/bep_0003.html


## Start a Tracker Server  

maybe you can use vps or aws ecp or gcp .. 
in this document, digital ocean vps (droplets)

https://www.digitalocean.com/

#### Install Opentracker

```
$ apt-get update
$ apt-get install build-essential -y
$ apt-get install git -y
$ apt-get install cvs -y
$ apt-get install zlib1g-dev -y
```

```
$ cvs -d :pserver:cvs@cvs.fefe.de:/cvs -z9 co libowfat
$ cd libowfat
$ make
$ cd ..
$ git clone git://erdgeist.org/opentracker
$ cd opentracker
$ make
```
#### RUN

```
./opentracker
```

You can get your Open Tracker Address!!
`http::<your vps ip>:6969/announce` is  your tracker address


## (Optional) Server Setting

You can start a tracker server. but, this server is stop if your server restart or your tracker server is down.

You can setup a tracker as systemd service.

#### Create Systemd Setting

/opt/my-opentracker.sh

```
#!/bin/sh

while true
do
  /root/opentracker/opentracker
  sleep 1
done

```

```
$ chmod 655 /opt/my-opentracker.sh
```

/etc/systemd/system/my-opentracker.service

```
[Unit]
Description = opentracker deamon

[Service]
ExecStart = /opt/my-opentracker.sh
Restart = always
Type = simple

[Install]
WantedBy = multi-user.target

```

```
$ systemctl enable my-codeserver
```


# MEMO

Dockerfile 's Sample is here
https://github.com/kyorohiro/hello_libtorrent
Dockerfile.tracker


# My Memo About Libtorrent And Index 
(0) code-server for libtorrent 2.0 
https://dev.to/kyorohiro/code-server-for-libtorrent-2-0-1m85

