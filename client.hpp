/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 15:06:20 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/15 17:39:37 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "includes.hpp"
#include "static/StatusCode.hpp"
#include "A_Request.hpp"
#include "GetRequest.hpp"
#include "PostRequest.hpp"
#include "parsing/configParser/Location.hpp"
#include "parsing/configParser/ServerConfigs.hpp"
// #include "server.hpp"

class A_Request; 
class PostRequest;
class GetRequest;
class ServerConfigs;
class Server;

#define ServerMap std::map<std::string, Server >

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
		unsigned long long 	received;
		StatusCode 	responseCode;
		bool		sendError;
		A_Request   *requestHandler;
		bool		requestHeaderDone;
		ServerConfigs	*requestConfigs;// reset if we reset request?
		bool 			body_done;
		Location		*bestLocationMatched;


		Client();
		
		Client(SOCKET socket);
		
		bool isRequestHeaderDone() const;

		ServerConfigs &getRequestConfigs()
		{
			return (*requestConfigs);
		}

		const char *get_address(); //return address client as string
		void set_response_code(StatusCode responseCode);

		void factoryRequestHandlerSetter();
		void set_request_configs(ServerConfigs	*requestConfigs_);
		void finished_body();
		bool body_is_done();
		void setBestLocationMatched();
};

#endif