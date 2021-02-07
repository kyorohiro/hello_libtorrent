[code-server deamon on your VPS]

Install Code-Server and Launch Code-Cerver At Server Restart Your VPS.


# install

```
curl -fsSL https://code-server.dev/install.sh | sh
```

# kick code-server shell

/opt/code-server.sh 

```
#!/bin/sh

while true
do
  PASSWORD=<Your Password> code-server --auth password --host 0.0.0.0 --port 8443 /app
  sleep 1
done

```

```
$ chmod 655 /opt/code-server.sh 
```

# service script

/etc/systemd/system/my-code-server.service

```
[Unit]
Description = code-server deamon

[Service]
ExecStart = /opt/code-server.sh
Restart = always
Type = simple

[Install]
WantedBy = multi-user.target
```

# start command

```
systemctl enable my-code-server
```
