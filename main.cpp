/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 16:34:04 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/05 16:00:39 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "parsing/configParser/ConfigParser.hpp"


std::string readFile(std::string file)
{
	std::ifstream myfile (file);

	std::string fileContents;
	
	std::string serverConfigs = "";
	if ( myfile.is_open() ) 
	{

		std::string sa;
        while (getline(myfile, sa))
		
            fileContents += sa + "\n";
        myfile.close();
	}
	return (fileContents);
};


void closeHosts(std::map<std::string, ServerMap > &servers)
{
	for (auto xs : servers)
	{
		for (auto ser : xs.second)
		{
			if (!ISVALIDSOCKET(ser.second.getSocket()))
				ser.second.closeHost();
		}
	}
}


int main(int ac , char **av)
{

	if (ac != 2)
	{
		std::cerr << "number argument Not valid !" << std::endl;
		return (0);
	}
	std::ifstream myfile (av[1]);
	ConfigParser parser = ConfigParser(readFile(av[1]));
	std::map<std::string, ServerMap > servers;
	SOCKET maxSocketSoFar = -1;
	fd_set reads , writes, readyReads, readWrites;
	FD_ZERO(&reads), FD_ZERO(&writes), FD_ZERO(&readWrites), FD_ZERO(&readyReads);
	servers = parser.parseServerConfig();
	for (auto &xs : servers)
	{
		for (auto &ser : xs.second)
		{
			try
			{
				ser.second.createSocket();
				std::cout << "socket is = " << ser.second.getSocket() << std::endl;
				FD_SET(ser.second.getSocket() , &reads);
				FD_SET(ser.second.getSocket() , &writes);
				maxSocketSoFar = std::max(maxSocketSoFar, ser.second.getSocket() );
			}
			catch(const std::exception& e)
			{
				closeHosts(servers);
				return (0);
			}
		}
		
	}

	while (1)
	{
		try
		{
			readyReads = reads;
			readWrites = writes;
			if (select(maxSocketSoFar + 1, &readyReads, &readWrites, 0, 0) < 0) {
				fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
				break;
			}
			for (auto &xs : servers)
			{
				for (auto &ser : xs.second)
				{
					Server &server  = ser.second;
					ListClients &clients = server.getClients();
					if (FD_ISSET(server.getSocket(), &readyReads))
						server.addClient(maxSocketSoFar, reads, writes);
					
					for (int  i = 0; i < clients.getNumberClient(); i++)
					{
						SOCKET clientSocket = clients[i].socket;
						if (FD_ISSET(clientSocket, &readyReads))
						{
							if (clients[i].received == MAX_REQUEST_SIZE)
							{
								server.send_400(clients[i]);
								FD_CLR(clientSocket, &reads);
								FD_CLR(clientSocket, &writes);
								i--;
								continue;
							}
							int sz = recv(clientSocket, clients[i].request + clients[i].received,MAX_REQUEST_SIZE -  clients[i].received, 0);
							if (sz < 1)
							{
								printf("Unexpected disconnect from %s.\n",
								clients[i].get_address());
								clients.dropClient(clientSocket);
								FD_CLR(clientSocket, &reads);
								FD_CLR(clientSocket, &writes);
								i--;
								continue;
							}
							clients[i].received += sz;
							clients[i].request[clients[i].received] = '\0';
							char *request = strstr(clients[i].request, "\r\n\r\n");
							if (request) { //if request is done
								*request = 0;
								if (strncmp("GET /", clients[i].request, 5))
								{
									server.send_400(clients[i]);
									FD_CLR(clientSocket, &reads);
									FD_CLR(clientSocket, &writes);
									i--;
									continue;
								}
								else 
								{
									char *path = clients[i].request + 4;
									char *end_path = strstr(path, " ");
									if (!end_path) 
									{
										server.send_400(clients[i]);
										FD_CLR(clientSocket, &reads);
										FD_CLR(clientSocket, &writes);
										i--;
										continue;
									} 
									else 
									{
										*end_path = 0;
										clients[i].path = path;
									}
								}
							}
						}
						if (FD_ISSET(clientSocket, &readWrites) && clients[i].path)
						{
							if (clients[i].fp == nullptr)
							{
								if (!server.sendHeaderResponse(clients[i]))
								{
									FD_CLR(clientSocket, &reads);
									FD_CLR(clientSocket, &writes);
									i--;
									continue;
								}
							}
							server.serve_resource(clients[i]);
							if (clients[i].fp == nullptr)
							{
								clients.dropClient(clientSocket);
								FD_CLR(clientSocket, &reads);
								FD_CLR(clientSocket, &writes);
								i--;
							}
						}
					}	
				}
			}
		}
		catch (std::exception &e)
		{
			std::cout << "error happend" << std::endl;
			break;
		}
	}
	closeHosts(servers);
    return 0;
}