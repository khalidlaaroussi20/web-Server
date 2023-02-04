/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:49:29 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/03 17:46:39 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#include "ListClients.hpp"
#include "parsing/configParser/ServerConfigs.hpp"



class Server
{
	private :
		ServerConfigs	_serverConfigs;
		SOCKET 			_serverSocket;
		SOCKET			_maxSocketSoFar;
		ListClients		_clients;
		
	public :
		Server()
		{
			_serverSocket = _maxSocketSoFar = INVALID_FD;
		}

		ServerConfigs	&getServerConfigs()
		{
			return (_serverConfigs);
		}

		const char *get_content_type(const char* path) {
			const char *last_dot = strrchr(path, '.');
			if (last_dot) {
				if (strcmp(last_dot, ".css") == 0) return "text/css";
				if (strcmp(last_dot, ".csv") == 0) return "text/csv";
				if (strcmp(last_dot, ".gif") == 0) return "image/gif";
				if (strcmp(last_dot, ".htm") == 0) return "text/html";
				if (strcmp(last_dot, ".html") == 0) return "text/html";
				if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
				if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
				if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
				if (strcmp(last_dot, ".js") == 0) return "application/javascript";
				if (strcmp(last_dot, ".json") == 0) return "application/json";
				if (strcmp(last_dot, ".png") == 0) return "image/png";
				if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
				if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
				if (strcmp(last_dot, ".txt") == 0) return "text/plain";
   			}
			return "application/octet-stream";
		}

		void createSocket()
		{
			struct addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			struct addrinfo *bind_address;
			getaddrinfo(_serverConfigs.getHost().c_str(), _serverConfigs.getServ().c_str(), \
			&hints, &bind_address);

			printf("Creating socket...\n");
			_serverSocket = socket(bind_address->ai_family,
					bind_address->ai_socktype, bind_address->ai_protocol);
			_maxSocketSoFar = _serverSocket;
			if (!ISVALIDSOCKET(_serverSocket)) {
				fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}

			printf("Binding socket to local address...\n");
			if (bind(_serverSocket,
						bind_address->ai_addr, bind_address->ai_addrlen)) {
				fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}
			freeaddrinfo(bind_address);

			printf("Listening...\n");
			if (listen(_serverSocket, 10) < 0) {
				fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}
		}

		void addClientSocket(std::pair<fd_set, fd_set> &ioSocket)
		{
			for (int i = 0; i < _clients.getNumberClient(); i++)
			{
				FD_SET(_clients[i].socket, &ioSocket.first);
				FD_SET(_clients[i].socket, &ioSocket.second);
			}
		}

		// return pair <fd_set reads , fd_set writes>
		std::pair<fd_set, fd_set> wait_on_clients() 
		{
			std::pair<fd_set, fd_set> p;
			FD_ZERO(&p.first);
			FD_ZERO(&p.second);
			FD_SET(_serverSocket, &p.first);
			
			addClientSocket(p);

			if (select(_maxSocketSoFar + 1, &p.first, &p.second, 0, 0) < 0) {
				fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}
			return p;
		}

		bool sendHeaderResponse(Client &client)
		{
			printf("serve_resource %s %s\n",client.get_address(), client.path);

			if (strcmp(client.path, "/") == 0) client.path = "/index.html";

			if (strlen(client.path) > 100) {
				send_400( client);
				return (false);
			}

			if (strstr(client.path, "..")) {
				send_404(client);
				return (false);
			}

			std::string full_path = "public";
			full_path += client.path;
			client.fp = fopen(full_path.c_str(), "rb");

			std::cout << "path = " << full_path << std::endl;
			if (!client.fp) {
				std::cout << "hereee\n";
				send_404(client);
				return (false);;
			}

			fseek(client.fp, 0L, SEEK_END);
			size_t fileSize = ftell(client.fp);
			rewind(client.fp);

			
			const char *contentType = get_content_type(full_path.c_str());
			char buffer[BUFFER_SIZE + 1];
			memset(buffer, 0, BUFFER_SIZE);

			sprintf(buffer, "HTTP/1.1 200 OK\r\n");
			send(client.socket, buffer, strlen(buffer), 0);

			sprintf(buffer, "Connection: close\r\n");
			send(client.socket, buffer, strlen(buffer), 0);

			sprintf(buffer, "Content-Length: %lu\r\n", fileSize);
			send(client.socket, buffer, strlen(buffer), 0);

			sprintf(buffer, "Content-Type: %s\r\n", contentType);
			send(client.socket, buffer, strlen(buffer), 0);

			sprintf(buffer, "\r\n");
			send(client.socket, buffer, strlen(buffer), 0);
			return (true);
		}

		void serve_resource(Client &client) {
			if (client.fp == nullptr)
				return ;
			char buffer[BUFFER_SIZE + 1];
			memset(buffer, 0, BUFFER_SIZE);
			int r = fread(buffer, 1, BUFFER_SIZE, client.fp);
			if (!r)
			{
				fclose(client.fp);
				client.fp = nullptr;
			}
			send(client.socket, buffer, r, 0);
		}

		void addClient()
		{
			Client newClient;
			newClient.socket = accept(_serverSocket, (struct sockaddr *)&(newClient.address), &newClient.address_length);
			 if (!ISVALIDSOCKET(newClient.socket)) {
                fprintf(stderr, "accept() failed. (%d)\n",
                        GETSOCKETERRNO());
                exit (1);
            }


            printf("New connection from %s.\n",
                    newClient.get_address());
			_clients.AddClient(newClient);
			_maxSocketSoFar = std::max(_maxSocketSoFar, newClient.socket);
		}

		void send_400(Client &client) {
			const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
				"Connection: close\r\n"
				"Content-Length: 11\r\n\r\nBad Request";
			send(client.socket, c400, strlen(c400), 0);
			_clients.dropClient(client.socket);
		}
		
		void send_404(Client &client) {
			 const char *c404 = "HTTP/1.1 404 Not Found\r\n"
        "Connection: close\r\n"
        "Content-Length: 9\r\n\r\nNot Found";
			send(client.socket, c404, strlen(c404), 0);
			_clients.dropClient(client.socket);
		}

		void handleRequest()
		{
			fd_set writes, reads;			
			std::pair <fd_set, fd_set> p = wait_on_clients();
			reads = p.first;
			writes = p.second;
			if (FD_ISSET(_serverSocket, &reads))
				addClient();
			for (int  i = 0; i < _clients.getNumberClient(); i++)
			{
				if (FD_ISSET(_clients[i].socket, &reads))
				{
					if (_clients[i].received == MAX_REQUEST_SIZE)
					{
						send_400(_clients[i]);
						i--;
						continue;
					}
					int sz = recv(_clients[i].socket, _clients[i].request + _clients[i].received,MAX_REQUEST_SIZE -  _clients[i].received, 0);
					if (sz < 1)
					{
						printf("Unexpected disconnect from %s.\n",
						_clients[i].get_address());
						_clients.dropClient(_clients[i].socket);
						i--;
						continue;
					}
					_clients[i].received += sz;
					_clients[i].request[_clients[i].received] = '\0';
					char *request = strstr(_clients[i].request, "\r\n\r\n");
                    if (request) { //if request is done
                        *request = 0;
                        if (strncmp("GET /", _clients[i].request, 5))
						{
                            send_400(_clients[i]);
							i--;
							continue;
                        }
						else 
						{
                            char *path = _clients[i].request + 4;
                            char *end_path = strstr(path, " ");
                            if (!end_path) 
							{
                                send_400(_clients[i]);
								i--;
								continue;
                            } 
							else 
							{
                                *end_path = 0;
								_clients[i].path = path;
                            }
                        }
                    }
				}
				if (FD_ISSET(_clients[i].socket, &writes) && _clients[i].path)
				{
					if (_clients[i].fp == nullptr)
					{
						if (!sendHeaderResponse(_clients[i]))
						{
							std::cout << "aaa\n";
							i--;
							continue;
						}
					}
					serve_resource(_clients[i]);
					if (_clients[i].fp == nullptr)
					{
						_clients.dropClient(_clients[i].socket);
						i--;
					}
				}
			}	
		}

		void closeServer()
		{
			printf("\nClosing socket...\n");
			CLOSESOCKET(_serverSocket);
			printf("Finished.\n");
		}
};


#endif