/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_Request.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 14:53:29 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/17 12:28:26 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A_Request.hpp"
#include "client.hpp"

A_Request::A_Request()
{
	_isErrorOccurs = false;
};

std::string &A_Request::getPath() 
{
	return (_path);
}

std::string &A_Request::getMethod() 
{
	return (_method);
}


void A_Request::setHeadersForCgi(std::string &request)
{
	HeaderParser parser(request);
	parser.getHeaderLine();
	
	while (!parser.isDoneParsing())
	{
		std::string headerLine = parser.getHeaderLine();
		if (!headerLine.empty())
			_headersForCgi.push_back(headerLine);
	}

}

void A_Request::parseRequestHeader(std::string &request)
{
	std::string keysDelimeters = ":";
	std::string valuesDelimeters = ";,=:";
	HeaderParser parser(request);
	_method = parser.getNextToken(keysDelimeters);
	_path = parser.getNextToken(keysDelimeters);
	_httpVersion =  parser.getNextToken(keysDelimeters);

	while (!parser.isDoneParsing())
	{
		std::string requestHeader = parser.getNextToken(keysDelimeters);
		if (!requestHeader.empty())
		{
			std::vector < std::string >  values = parser.getValuesCurrToken(valuesDelimeters);
			_headers[requestHeader] = values;
			if (requestHeader == "Content-Type")
			{
				if (values.size() >= 3)
				{
					if (values[0] == "multipart/form-data")
					{
						if (values[1] != "boundary")
							_isErrorOccurs = true;
						else
							_headers["boundary"] = std::vector <std::string> (1,values[2]);
					}
				}
			}
		}
	}
	setHeadersForCgi(request);
	// for (int i = 0; i < _headersForCgi.size(); i++)
	// {
	// 	std::cout << "aa " << _headersForCgi[i] << std::endl;
	// }
	
}

std::string &A_Request::getHttpVersion()
{
	return (_httpVersion);
}

A_Request::headersType &A_Request::getHeaders()
{
	return (_headers);
}

std::string A_Request::getHeaderValue(const std::string header)
{
	std::string headerValue;
	auto it = _headers.find(header);
	if (it !=_headers.end() && !it->second.empty())
		headerValue = it->second[0];
	return (headerValue); 
}

bool A_Request::isRequestWellFormed(Client &client)
{
	bool isWellFormed = false;
	std::string transferEncoding = getHeaderValue("Transfer-Encoding");
	if (!transferEncoding.empty() && transferEncoding != "chunked")
	{
		client.set_response_code(NOT_IMPLEMENTED);
		return (false);
	}
	std::string contentLength = getHeaderValue("Content-Length");
	if (transferEncoding.empty() && contentLength.empty() && _method == "POST")
	{
		client.set_response_code(BAD_REQUEST);
		return (false);
	}
	if (_path.length() > MAX_URI_SIZE)
	{
		client.set_response_code(REQUEST_URI_TOO_LONG);
		return (false);
	}
	if (client.bestLocationMatched && !client.bestLocationMatched->getRedirect().empty())
	{
		client.set_response_code(MOVED_PERMANETLY);
		return (false);
	}

	isWellFormed = true;
	return (isWellFormed);
}

//aaa/a..

bool A_Request::isValidPath()
{
	std::stringstream stream(_path + "/");
	std::string token;
	while (std::getline(stream, token, '/')){
		if (token == ".." || token == "."){
			return (false);
		}
	}
	return (true);
}