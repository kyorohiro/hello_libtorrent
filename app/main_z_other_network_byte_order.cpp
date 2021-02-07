#include <iostream>

// headers for socket(), getaddrinfo() and friends
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>    // close()

//
// Little-endian in Intel microprocessors
// Big-endian in Motorola microprocessors and Network Byte order
// 
// #include <arpa/inet.h>
// uint32_t htonl(uint32_t hostlong);
// uint16_t htons(uint16_t hostshort);
// uint32_t ntohl(uint32_t netlong);
// uint16_t ntohs(uint16_t netshort);
//
// $ g++ main_z_other_network_byte_order.cpp 
// $ ./a.out
// 8080
// 36895
// 0001,1111,1001,0000, # little endian
// 1001,0000,0001,1111, # big endian
// 1001,0000,0001,1111, # big endian
//
uint16_t convert_endian(uint16_t src) {
    int ret = 0;
    ret |= (src >> 8) & 0x00ff;
    ret |= (src << 8) & 0xff00;
    return ret;
}
int main(int argc, char* argv[]) {
    std::cout << 8080 << std::endl;
    std::cout << htons(8080) << std::endl;
    for(int i=0;i<16;i++) {
        std::cout << ((8080>>(15-i))&0x01);
        if((i+1)%4==0) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
    for(int i=0;i<16;i++) {
        std::cout << (( htons(8080)>>(15-i))&0x01);
         if((i+1)%4==0) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
    for(int i=0;i<16;i++) {
        std::cout << (( convert_endian(8080)>>(15-i))&0x01);
         if((i+1)%4==0) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
    return 0;
}