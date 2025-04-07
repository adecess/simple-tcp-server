#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_CLIENTS 256
#define PORT 8080
#define BUFF_SIZE 4096

typedef enum
{
    STATE_NEW,
    STATE_CONNECTED,
    STATE_DISCONNECTED
} state_e;

// Structure to hold client state
typedef struct
{
    int fd;
    state_e state;
    char buffer[4096];
} clientstate_t;

clientstate_t clientStates[MAX_CLIENTS];

void init_clients()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clientStates[i].fd = -1; // -1 indicates a free slot
        clientStates[i].state = STATE_NEW;
        memset(&clientStates[i].buffer, '\0', BUFF_SIZE);
    }
}

int find_free_slot()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clientStates[i].fd == -1)
        {
            return i;
        }
    }
    return -1; // No free slot found
}

// Main function
int main()
{
}