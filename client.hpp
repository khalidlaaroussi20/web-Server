/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:06:20 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/06 18:31:26 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "includes.hpp"


class Client
{
	public :
		socklen_t address_length;
		struct sockaddr_storage address;
		char address_buffer[128];
		SOCKET socket;
		char request[MAX_REQUEST_SIZE + 1];
		char *path;
		FILE *fp;
		int received;
		int responseCode;
	
	Client()
	{
		received = 0;
		address_length = sizeof(address);
		socket = -1;
		path  = nullptr;
		fp = nullptr;
		responseCode = 200;
	}
	Client(SOCKET socket)
	{
		received = 0;
		address_length = sizeof(address);
		path  = nullptr;
		fp = nullptr;
		this->socket = socket;
		responseCode = 200;
	}

	const char *get_address() //return address client as string
		{
			getnameinfo((struct sockaddr*)&address,
					address_length,
					address_buffer, sizeof(address_buffer), 0, 0,
					NI_NUMERICHOST);
			return (address_buffer);
		}
	
	
	
};

#endif