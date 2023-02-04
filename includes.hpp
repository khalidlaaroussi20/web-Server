#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>



#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#define INVALID_FD -1
#define CLIENT_NOT_FOUND -1
#define FAILURE 0
#define SUCCESS 1

#define INFINI 0


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <iostream>
#include <fstream>


#define MAX_REQUEST_SIZE 2047
#define BUFFER_SIZE		 1024

#endif