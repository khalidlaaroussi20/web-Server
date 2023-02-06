/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 16:34:04 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/06 19:04:50 by klaarous         ###   ########.fr       */
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
	fd_set reads , writes, readyReads, readyWrites;
	FD_ZERO(&reads), FD_ZERO(&writes), FD_ZERO(&readyWrites), FD_ZERO(&readyReads);
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
			readyWrites = writes;
			if (select(maxSocketSoFar + 1, &readyReads, &readyWrites, 0, 0) < 0) {
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
					for (int i = 0; i < clients.getNumberClient(); i++)
					{
						int sizeClient = clients.getNumberClient();
						if (FD_ISSET(clients[i].socket, &readyReads))
						{
							if (clients[i].received == MAX_REQUEST_SIZE)
							{
								printf("400 %d.\n",
								clients[i].socket );
								clients.dropClient(i, reads, writes);
								//server.send_400(clients[i], reads, writes, i);
							}
							else
							{
								int sz = recv(clients[i].socket, clients[i].request + clients[i].received,MAX_REQUEST_SIZE -  clients[i].received, 0);
								if (sz < 1)
								{
									printf("Unexpected disconnect from %d.\n",
									clients[i].socket );
									clients.dropClient(i, reads, writes);
								}
								else
								{
									clients[i].received += sz;
									clients[i].request[clients[i].received] = '\0';
									char *request = strstr(clients[i].request, "\r\n\r\n");
									if (request) 
									{ //if request is done
										*request = 0;
										if (strncmp("GET /", clients[i].request, 5))
										{
											printf("400 %d.\n",
											clients[i].socket );
											clients.dropClient(i, reads, writes);
											//server.send_400(clients[i], reads, writes, i);
										}
										else 
										{
											char *path = clients[i].request + 4;
											char *end_path = strstr(path, " ");
											if (!end_path) 
											{
												printf("400 %d.\n",
												clients[i].socket );
												clients.dropClient(i, reads, writes);
												//server.send_400(clients[i], reads, writes, i);
											} 
											else 
											{
												*end_path = 0;
												clients[i].path = path;
											}
										}
									}
								}
							}
						}
						if (i < 0)
							std::cout << "i = " << i << std::endl;
						if (i >= 0  && FD_ISSET(clients[i].socket, &readyWrites) && clients[i].path)
						{
							if (clients[i].fp == nullptr)
							{
								server.sendHeaderResponse(clients[i], reads, writes, i);
							}
							server.serve_resource(clients[i]);
							if (clients[i].fp == nullptr)
							{
								clients.dropClient(i, reads, writes);
							}
						}
					}	
				}
			}
		}
		catch (std::exception &e)
		{
			std::cout << "error happend " <<  e.what() << std::endl;
			break;
		}
	}
	closeHosts(servers);
    return 0;
}