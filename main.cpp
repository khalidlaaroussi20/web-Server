/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 16:34:04 by klaarous          #+#    #+#             */
/*   Updated: 2023/01/30 14:38:57 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


int main()
{
	char *host = nullptr;
	char *port = "8080";
	Server server(host, port);
	try
	{
		server.createSocket();
	}
	catch(const std::exception& e)
	{
		server.closeServer();
		return (0);
	}
	while (1)
	{
		try
		{
			server.handleRequest();
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			break;
		}
	}
	server.closeServer();

    return 0;
}