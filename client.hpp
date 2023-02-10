/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:06:20 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/10 15:02:02 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "includes.hpp"
#include "static/StatusCode.hpp"
#include "GetRequest.hpp"
#include "PostRequest.hpp"


class A_Request; 

class Client
{
	public :
		socklen_t address_length;
		struct sockaddr_storage address;
		char 	address_buffer[128];
		SOCKET 	socket;
		char 	request[MAX_REQUEST_SIZE + 1];
		char 	*path;
		FILE 		*fp;
		int 		received;
		int 		responseCode;
		bool		sendError;
		A_Request   *requestHandler;
		bool		requestHeaderDone;
	
		Client();
		
		Client(SOCKET socket);
		
		bool isRequestHeaderDone() const;

		const char *get_address(); //return address client as string
		void set_error_code(int errorCode);

		void factoryRequestHandlerSetter();
	
};

#endif