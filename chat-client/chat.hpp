#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE_BUFFER 4096
#define PORT 55555

void chat();