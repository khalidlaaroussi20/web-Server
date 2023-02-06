/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:49:29 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/06 19:03:51 by klaarous         ###   ########.fr       */
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
		ListClients		_clients;
		
	public :
		Server()
		{
			_serverSocket  = INVALID_FD;
		}

		ServerConfigs	&getServerConfigs()
		{
			return (_serverConfigs);
		}

		SOCKET getSocket() const
		{
			return (_serverSocket);
		}

		ListClients &getClients()
		{
			return (_clients);
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
			if (!ISVALIDSOCKET(_serverSocket)) {
				fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}
			fcntl(_serverSocket, F_SETFL,  O_NONBLOCK);
			printf("Binding socket to local address...\n");
			if (bind(_serverSocket,
						bind_address->ai_addr, bind_address->ai_addrlen)) {
				fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
				exit(1);
			}
			freeaddrinfo(bind_address);

			printf("Listening...\n");
			if (listen(_serverSocket, 10000) < 0) {
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

			// if (select(_maxSocketSoFar + 1, &p.first, &p.second, 0, 0) < 0) {
			// 	fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
			// 	exit(1);
			// }
			return p;
		}

		Location &getBestMatchedLocation(std::vector <Location> &locations, std::string path)
		{
			int idxBestLocation = 0;
			int maxLenMatched = 1;
			for (int i = locations.size() - 1 ; i > 0; i--)
			{
				if (locations[i].isRouteMatch(path))
				{
					if (locations[i].getRoute().length() >= maxLenMatched)
					{
						maxLenMatched = locations[i].getRoute().length();
						idxBestLocation = i;
					}
				}
			}
			std::cout << "best InDEX = " << idxBestLocation << std::endl;
			return (locations[idxBestLocation]);
		}
		
		std::string getPathRessource(Location &bestLocationMatched, std::string path)
		{
			std::string currPath = bestLocationMatched.getRoot();
			if (currPath[currPath.length() - 1] != '/')
				currPath += "/";
			currPath += path.substr(bestLocationMatched.getRoute().length(), path.length());
			std::cout << "currPath1 = " << currPath << std::endl;
			return (currPath); 
		}

		bool  tryOpenRessource(Location &location , std::string &path, Client &client)
		{
			struct stat s;
			if( stat(path.c_str(),&s) == 0 )
			{
				if( s.st_mode & S_IFDIR )
				{
					std::cout << "is Directory\n";
					//it's a directory
					std::vector <std::string> &indexes =  location.getIndexes();
					for (int i = 0; i < indexes.size();i++)
					{
						std::string fullPath = path + indexes[i];
						std::cout << "fullPath = " << fullPath << std::endl;
						if (fullPath.length() < 100)
						{
							client.fp = fopen(fullPath.c_str(), "rb");
							if (client.fp)
							{
								path = fullPath.c_str();
								return (true);
							}
						}
					}
					return false;
					
				}
				else if( s.st_mode & S_IFREG )
				{
					//it's a file
					client.fp = fopen(path.c_str(), "rb");
					return (client.fp);
				}
			}
			return (false);
		}

		bool sendHeaderResponse(Client &client, fd_set &reads, fd_set &writes, int &clientIdx)
		{
			std::cout << "path == " << client.path << std::endl;
			Location &bestLocationMatched = getBestMatchedLocation(_serverConfigs.getLocations(), client.path);
			std::cout << "bestroute = " << bestLocationMatched.getRoute() << std::endl;
			std::cout << "bestroot = " << bestLocationMatched.getRoot() << std::endl;
			std::vector<std::string> indexes = bestLocationMatched.getIndexes();
			for (int i = 0; i < indexes.size();i++)
			{
				std::cout << " " << indexes[i];
			}
			std::cout << std::endl;
			std::string path = getPathRessource(bestLocationMatched ,client.path);
			std::cout << "fullPath = " << path << std::endl;
			if (path.find("..") != std::string::npos)
			{
				send_400( client, reads, writes, clientIdx);
				return (false);
			}

			if (tryOpenRessource(bestLocationMatched, path, client) == false)
			{
				send_404(client, reads, writes, clientIdx);
				return (false);;
			}
			
			printf("serve_resource %s %s\n",client.get_address(), client.path);


			std::cout << "path = " << path << std::endl;
			fseek(client.fp, 0L, SEEK_END);
			size_t fileSize = ftell(client.fp);
			rewind(client.fp);

			
			const char *contentType = get_content_type(path.c_str());
			char buffer[BUFFER_SIZE + 2];
			memset(buffer, 0, BUFFER_SIZE + 1);
			
			sprintf(buffer, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %lu\r\nContent-Type: %s\r\n\r\n", fileSize, contentType);
			std::cout << "before send 3 to " << client.socket << std::endl;
			if (send(client.socket, buffer, strlen(buffer), 0)  == -1)
			{
				std::cout << "error3" << std::endl;
				fclose(client.fp);
				client.fp = nullptr;
				_clients.dropClient(clientIdx, reads, writes);
				return (false);
			}
			std::cout << "after send 3 to " << client.socket << std::endl;
			return (true);
		}

		void serve_resource(Client &client) {
			if (client.fp == nullptr)
				return ;
			char buffer[BUFFER_SIZE + 2];
			memset(buffer, 0, BUFFER_SIZE + 1);
			int r = fread(buffer, 1, BUFFER_SIZE, client.fp);
			if (r <= 0 || send(client.socket, buffer, r + 1, 0) == -1)
			{
				fclose(client.fp);
				client.fp = nullptr;
				return ;
			}
			std::cout << "before send 4 to " << client.socket << std::endl;
			if (send(client.socket, buffer, r + 1, 0) == -1)
			{
				std::cout << "error4 " << std::endl;
				fclose(client.fp);
				client.fp = nullptr;
			}
			std::cout << "after send 4 to " << client.socket << std::endl;
		}

		void addClient(SOCKET &maxSocketSoFar, fd_set &reads, fd_set &writes)
		{
			Client newClient;
			newClient.socket = accept(_serverSocket, (struct sockaddr *)&(newClient.address), &newClient.address_length);
			 if (!ISVALIDSOCKET(newClient.socket)) {
                fprintf(stderr, "accept() failed. (%d)\n",
                        GETSOCKETERRNO());
				return ;
            }

            printf("New connection from %d.\n",
                  newClient.socket);
			_clients.AddClient(newClient);
			fcntl(newClient.socket, F_SETFL,  O_NONBLOCK);
			FD_SET(newClient.socket, &reads);
			FD_SET(newClient.socket, &writes);
			maxSocketSoFar = std::max(maxSocketSoFar, newClient.socket);
			std::cout << "client added \n";
		}

		void send_400(Client &client, fd_set &reads, fd_set &writes, int &clientIdx) {
			const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
				"Connection: close\r\n"
				"Content-Length: 11\r\n\r\nBad Request";
			std::cout << "before send 2" << std::endl;
			if (send(client.socket, c400, strlen(c400), 0) == -1)
			{
				std::cout << "error 2" << std::endl;
			}
			std::cout << "after send 2" << std::endl;
			_clients.dropClient(clientIdx, reads, writes);
		}
		
		void send_404(Client &client, fd_set &reads, fd_set &writes, int &clientIdx) {
			 const char *c404 = "HTTP/1.1 404 Not Found\r\n"
        "Connection: close\r\n"
        "Content-Length: 9\r\n\r\nNot Found";
			std::cout << "before send 1" << std::endl;
			if (send(client.socket, c404, strlen(c404), 0) == -1)
			{
				std::cout << "error 3" << std::endl;
			}
			std::cout << "after send 1" << std::endl;
			_clients.dropClient(clientIdx, reads, writes);
		}

		void closeHost()
		{
			printf("\nClosing socket...\n");
			CLOSESOCKET(_serverSocket);
			printf("Finished.\n");
		}
};


#endif