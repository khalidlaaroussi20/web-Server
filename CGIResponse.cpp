/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:42:23 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/24 15:26:52 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIResponse.hpp"


CGIResponse::CGIResponse()
{
	
}

void CGIResponse::_setBody(std::string body)
{
	_body = body;
}



void CGIResponse::_parseCgiHeader(std::string buffer ,Client &client)
{
	size_t pos = buffer.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		client.set_response_code(BAD_REQUEST);
		return ;
	}
	_header = buffer.substr(0, pos);
	this->_setBody(buffer.substr(pos + 4, buffer.length()));
	HeaderParser parser(_header);
	std::string keysDelimeters = ":";
	std::string valuesDelimeters = ";,=:";
	bool isErrorOccurs = false;
	while (!parser.isDoneParsing())
	{
		std::string requestHeader = parser.getNextToken(keysDelimeters);
		if (!requestHeader.empty())
		{
			std::vector < std::string >  values = parser.getValuesCurrToken(valuesDelimeters, isErrorOccurs);
			if (isErrorOccurs)
			{
				client.set_response_code(BAD_REQUEST);
				return ;
			}
			_cgiHeaders[requestHeader] = values;
		}
	}
}


std::string CGIResponse::getHeaderResponse(Client &client)
{
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE + 1);
	fread(buffer, 1,BUFFER_SIZE, client.fp);
	_parseCgiHeader(buffer, client);
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
	headerRespone += _body;
	return (headerRespone);
}