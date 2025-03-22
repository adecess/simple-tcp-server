#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>

int main() {
    // same as struct sockaddr but adds named fields that can be filled in
    // we use {0} to zero out the struct
    struct sockaddr_in serverInfo = { 0 };
    serverInfo.sin_family = AF_INET; // the socket type/family is ipv4
    serverInfo.sin_addr.s_addr = 0; // address from which we listen for connections, 0 means every ip address on our machine
    serverInfo.sin_port = htons(5555); // port number in network endian

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    // bind

    // accept

    // listen
}