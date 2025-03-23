#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // same as struct sockaddr but adds named fields that can be filled in
    // we use {0} to zero out the struct
    struct sockaddr_in serverInfo = { 0 };
    struct sockaddr_in clientInfo = { 0 };
    socklen_t clientSize = 0;

    serverInfo.sin_family = AF_INET; // the socket type/family is ipv4
    serverInfo.sin_addr.s_addr = 0; // address from which we listen for connections, 0 means every ip address on our machine
    serverInfo.sin_port = htons(5555); // port number in network endian

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    // bind
    // typecast &serverInfo to a struct sockaddr*
    if (bind(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
        perror("bind");
        close(fd);
        return -1;
    };

    // listen
    if (listen(fd, 0) == -1) {
        perror("listen");
        close(fd);
        return -1;
    }

    // accept
    // blocks the program's execution and waits for a connection to be taken in
    // when it does it returns a new fd that respresents the connection between the 2 computers
    int cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
    if (cfd == -1) {
        perror("accept");
        close(fd);
        return -1;
    }

    close(cfd);
}