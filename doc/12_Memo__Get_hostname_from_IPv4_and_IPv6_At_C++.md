
[Memo : Get hostname from IPv4 and IPv6 At C++ ]

This is just just a note for me...
for my bittorrent client tools
 

For IP v4

```
// 
// copy from https://stackoverflow.com/questions/28566424/linux-networking-gethostbyaddr 
//
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    struct sockaddr_in sa;    /* input */
    socklen_t len;         /* input */
    char hbuf[NI_MAXHOST];

    memset(&sa, 0, sizeof(struct sockaddr_in));

    /* For IPv4*/
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("8.8.8.8");
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *) &sa, len, hbuf, sizeof(hbuf), 
        NULL, 0, NI_NAMEREQD)) {
        printf("could not resolve hostname\n");
    }
    else {
        printf("host=%s\n", hbuf);
    }

    return 0;                                                  
}

```

For IP v6
```
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    struct sockaddr_in6 sa6;    
    char hbuf[NI_MAXHOST];
    memset(&sa6, 0, sizeof(struct sockaddr_in6));

    sa6.sin6_family = AF_INET6;
    in6_addr addr6;
    int s = inet_pton(AF_INET6, "::1",&addr6);
    sa6.sin6_addr = addr6;

    int len = sizeof(struct sockaddr_in6);
    if (getnameinfo((struct sockaddr *) &sa6, len, hbuf, sizeof(hbuf), 
        NULL, 0, NI_NAMEREQD)) {
        printf("could not resolve hostname\n");
    }
    else {
        printf("host=%s\n", hbuf);
    }

    return 0;                                                  
}
```

# CODE

https://github.com/kyorohiro/hello_libtorren
app/main_get_host_from_ipv6.cpp