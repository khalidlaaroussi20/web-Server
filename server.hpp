/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:49:29 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/07 16:09:26 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#include "ListClients.hpp"
#include "parsing/configParser/ServerConfigs.hpp"

#include "StatusCode.hpp"



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

		void setSocket(SOCKET socket) 
		{
			_serverSocket = socket;
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
			//std::cout << "best InDEX = " << idxBestLocation << std::endl;
			return (locations[idxBestLocation]);
		}
		
		std::string getPathRessource(Location &bestLocationMatched, std::string path)
		{
			std::string currPath = bestLocationMatched.getRoot();
			if (currPath[currPath.length() - 1] != '/')
				currPath += "/";
			currPath += path.substr(bestLocationMatched.getRoute().length(), path.length());
			//std::cout << "currPath1 = " << currPath << std::endl;
			return (currPath); 
		}

		void  tryOpenRessource(std::string &path, Client &client)
		{
			Location &bestLocationMatched = getBestMatchedLocation(_serverConfigs.getLocations(), client.path);
			path = getPathRessource(bestLocationMatched ,client.path);
			if (path.find("..") != std::string::npos)
			{
				client.responseCode = 400;
				return ;			
			}
			struct stat s;
			if( stat(path.c_str(),&s) == 0 )
			{
				if( s.st_mode & S_IFDIR )
				{
					//std::cout << "is Directory\n";
					//it's a directory
					std::vector <std::string> &indexes =  bestLocationMatched.getIndexes();
					for (int i = 0; i < indexes.size();i++)
					{
						std::string fullPath = path + indexes[i];
						//std::cout << "fullPath = " << fullPath << std::endl;
						if (fullPath.length() < 100)
						{
							client.fp = fopen(fullPath.c_str(), "rb");
							if (client.fp)
							{
								path = fullPath.c_str();
								break;
							}
						}
					}
					
				}
				else if( s.st_mode & S_IFREG )
				{
					//it's a file
					client.fp = fopen(path.c_str(), "rb");
				}
			}
			if (client.fp == nullptr)
				client.responseCode = NOT_FOUND;
		}
		
		std::string  getHeaderResponse(Client &client, std::string &path)
		{
			std::string headerRespone = "HTTP/1.1 " + std::to_string(client.responseCode);
			fseek(client.fp, 0L, SEEK_END);
			size_t fileSize = ftell(client.fp);
			rewind(client.fp);
			
			const char *contentType = get_content_type(path.c_str());
			if (client.responseCode == 400)
				headerRespone += "  Bad Request ";
			else if (client.responseCode == 404)
				headerRespone += "  Not Found ";
			else
				headerRespone += " OK ";
			headerRespone += "\r\nConnection: close\r\nContent-Length: " + std::to_string(fileSize) +  "\r\nContent-Type: " + contentType + "\r\n\r\n";
	
			return (headerRespone);
		}
		
		void setPathError(Client &client, std::string &path)
		{
			std::cout << "path before = " << path << " resCode = " << client.responseCode << std::endl;
			path = getServerConfigs().getErrorPage(client.responseCode);
			std::cout << "path Error = " << path << std::endl;
			client.fp = fopen(path.c_str(), "rb");
		}


		void sendHeaderResponse(Client &client, fd_set &reads, fd_set &writes, int &clientIdx)
		{
			std::string path = client.path;
			
			if (!client.sendError)
				tryOpenRessource(path, client);
			if (client.responseCode != OK)
				setPathError(client, path);
			std::string responseHeader = getHeaderResponse(client, path);			
			if (send(client.socket, responseHeader.c_str(), responseHeader.length(), 0)  == -1)
			{
				fclose(client.fp);
				client.fp = nullptr;
				_clients.dropClient(clientIdx, reads, writes);
			}
		}

		void serve_resource(Client &client) {
			if (client.fp == nullptr)
				return ;
			char buffer[BUFFER_SIZE + 1];
			memset(buffer, 0, BUFFER_SIZE + 1);
			int r = fread(buffer, 1, BUFFER_SIZE, client.fp);
			if (r <= 0)
			{
				fclose(client.fp);
				client.fp = nullptr;
				return ;
			}
			if (send(client.socket, buffer, r, 0) == -1)
			{
				fclose(client.fp);
				client.fp = nullptr;
			}
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
			send(client.socket, c400, strlen(c400), 0);
			_clients.dropClient(clientIdx, reads, writes);
		}
		
		void send_404(Client &client, fd_set &reads, fd_set &writes, int &clientIdx) {
			 const char *c404 = "HTTP/1.1 404 Not Found\r\n"
        	"Connection: close\r\n"
        	"Content-Length: 9\r\n\r\nNot Found";
			send(client.socket, c404, strlen(c404), 0);
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