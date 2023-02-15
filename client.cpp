/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:38:10 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/15 17:24:36 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client()
{
	received = 0;
	address_length = sizeof(address);
	socket = -1;
	path  = nullptr;
	fp = nullptr;
	responseCode = OK;
	sendError = false;
	requestHeaderDone = false;
	requestHandler = nullptr;
	body_done = false;
	bestLocationMatched= nullptr;
}


Client::Client(SOCKET socket)
{
	received = 0;
	address_length = sizeof(address);
	path  = nullptr;
	fp = nullptr;
	this->socket = socket;
	responseCode = OK;
	sendError = false;
	requestHeaderDone = false;
	requestHandler = nullptr;
	body_done = false;
	bestLocationMatched= nullptr;

}


bool Client::isRequestHeaderDone() const
{
	return (requestHeaderDone);
}

const char *Client::get_address() //return address client as string
{
	getnameinfo((struct sockaddr*)&address,
			address_length,
			address_buffer, sizeof(address_buffer), 0, 0,
			NI_NUMERICHOST);
	return (address_buffer);
}
void Client::set_response_code(StatusCode responseCode)
{
	this->responseCode = responseCode;
	sendError = true;
}


void Client::factoryRequestHandlerSetter()
{
	if (strncmp("GET /", request, 5) == 0)
		requestHandler = new GetRequest();
	else if (strncmp("POST /", request, 5) == 0)
		requestHandler = new PostRequest();
	else if (strncmp("DELETE /", request, 5) == 0)
		requestHandler = new GetRequest();
	else
	{
		requestHandler = new GetRequest();
		set_response_code(METHOD_NOT_ALLOWED);
		finished_body();
	}
	
}

void Client::set_request_configs(ServerConfigs	*requestConfigs_)
{
	requestConfigs = requestConfigs_;
};

void Client::finished_body()
{
	body_done = true;
};

void Client::setBestLocationMatched()
{
	if (requestConfigs)
	{
		std::string currPath = path;
		bestLocationMatched = &(requestConfigs->getBestMatchedLocation(currPath));
	}
	
}

bool Client::body_is_done()
{
	return body_done;
}