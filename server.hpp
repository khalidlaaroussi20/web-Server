/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:49:29 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/23 14:16:23 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#include "includes.hpp"
#include "static/ContentTypes.hpp"
#include "static/StaticResponseMessages.hpp"
#include "ListClients.hpp"

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

		Location &getBestMatchedLocation(std::string path)
		{
			return (_serverConfigs.getBestMatchedLocation(path));
		}
		

		std::string genereteRandomName()
		{
			std::time_t ms = std::time(nullptr);
			std::stringstream ss;
			ss << ms;
			return (ss.str());
		}
		

		
		std::string  getHeaderResponse(Client &client)
		{
			std::string headerRespone = client.requestHandler->getHttpVersion() + " " +  std::to_string(client.responseCode);
			fseek(client.fp, 0L, SEEK_END);
			size_t fileSize = ftell(client.fp);
			rewind(client.fp);
			std::string extention = FileSystem::getExtention(client.path);
			std::string contentType =  ContentTypes::getContentType(extention);
			headerRespone += StaticResponseMessages::getMessageResponseCode(client.responseCode);
			if (client.responseCode == MOVED_PERMANETLY)
				headerRespone += "\r\nLocation: " + client.bestLocationMatched->getRedirect();
			headerRespone += "\r\nConnection: close\r\nContent-Length: " + std::to_string(fileSize) +  "\r\nContent-Type: " + contentType + "; charset=utf-8 \r\n\r\n";
			return (headerRespone);
		}
		
	

		bool isRedirection(StatusCode &responseCode) const
		{
			return (responseCode >= 300 && responseCode < 400);
		}
		bool sendHeaderResponse(Client &client, fd_set &reads, fd_set &writes, int &clientIdx)
		{
			if (client.sendError)
				client.setPathResponse();
			std::string responseHeader = getHeaderResponse(client);
			std::cout << responseHeader << std::endl;	
			if (send(client.socket, responseHeader.c_str(), responseHeader.length(), 0)  == -1 ||\
			 		client.requestHandler->getMethod() == "HEAD")// || isRedirection(client.responseCode))
			{
				fclose(client.fp);
				client.fp = nullptr;
				_clients.dropClient(clientIdx, reads, writes);
				return (false);
			}
			client.isHeaderSend = true;
			return (true);
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
			newClient.socket = accept(_serverSocket, (struct sockaddr *)&(newClient.addresStorage), &newClient.address_length);
			 if (!ISVALIDSOCKET(newClient.socket)) {
                fprintf(stderr, "accept() failed. (%d)\n",
                        GETSOCKETERRNO());
				return ;
            }
			newClient.setClientInfo();
            printf("New connection from %d.\n",
                  newClient.socket);
			_clients.AddClient(newClient);
			fcntl(newClient.socket, F_SETFL,  O_NONBLOCK);
			FD_SET(newClient.socket, &reads);
			FD_SET(newClient.socket, &writes);
			maxSocketSoFar = std::max(maxSocketSoFar, newClient.socket);
		}

		void closeHost()
		{
			printf("\nClosing socket...\n");
			CLOSESOCKET(_serverSocket);
			printf("Finished.\n");
		}
};


#endif