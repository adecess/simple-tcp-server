#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum {
  PROTO_HELLO,
} proto_type_e;

// TLV
typedef struct {
  proto_type_e type;
  unsigned short len;
} proto_hdr_t;

void handle_server(int fd) {
  char buf[4096] = {0};
  read(fd, buf, sizeof(proto_hdr_t) + sizeof(int));

  proto_hdr_t *hdr = buf;
  hdr->type = ntohl(hdr->type);  // unpack the type
  hdr->len = ntohs(hdr->len);

  int *data = &hdr[1];
  *data = ntohl(*data);  // protocol version one, packed

  if (hdr->type != PROTO_HELLO) {
    printf("Protocol mismatch, failing.");
    return;
  }

  if (*data != 1) {
    printf("Protocol version mismatch, failing.");
    return;
  }

  printf("Server connected, protocol v1.\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <ip of the host>\n", argv[0]);
    return 0;
  }

  struct sockaddr_in serverInfo = {0};

  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
  serverInfo.sin_port = htons(5555);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    perror("socket");
    return -1;
  }

  if (connect(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("connect");
    close(fd);
    return 0;
  }

  handle_server(fd);

  close(fd);
}