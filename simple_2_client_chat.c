#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT1 8080
#define PORT2 8081
#define BUFFER_SIZE 1024

int create_server_socket(int port) {
  int server_fd;
  struct sockaddr_in address = {0};
  int opt = 1;

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options to reuse address and port
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", port);
  return server_fd;
}

int main() {
  int server_socket1, server_socket2;
  struct pollfd pfds[2];
  char buffer[BUFFER_SIZE];

  server_socket1 = create_server_socket(PORT1);
  server_socket2 = create_server_socket(PORT2);

  pfds[0].fd = server_socket1;
  pfds[0].events = POLLIN;  // monitor for incoming data

  pfds[1].fd = server_socket2;
  pfds[1].events = POLLIN;  // monitor for incoming data

  printf("Waiting for connections on ports %d and %d...\n", PORT1, PORT2);

  // Main server loop
  while (1) {
    int num_events = poll(pfds, 2, -1);  // -1 means wait indefinitely

    if (num_events < 0) {
      perror("poll error");
      exit(EXIT_FAILURE);
    }

    // Check which socket has activity
    for (int i = 0; i < 2; i++) {
      if (pfds[i].revents & POLLIN) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_socket;

        // Accept the incoming connections
        if ((client_socket = accept(pfds[i].fd, (struct sockaddr *)&client_addr,
                                    &addr_len)) < 0) {
          perror("accept failed");
          continue;
        }

        printf("New connection from %s:%d on port %d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),
               (pfds[i].fd == server_socket1 ? PORT1 : PORT2));

        // Read data from the client
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
          buffer[bytes_read] = '\0';  // null terminate the received data
          printf("Received from client on port %d: %s\n",
                 (pfds[i].fd == server_socket1 ? PORT1 : PORT2), buffer);

          write(client_socket, buffer,
                bytes_read);  // echo message back to client

          close(client_socket);
        }
      }
    }
  }

  // Close server sockets (this code is not reached in this example)
  close(server_socket1);
  close(server_socket2);

  return 0;
}