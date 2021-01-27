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
