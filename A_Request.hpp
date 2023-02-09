/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:21 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/08 14:08:16 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_REQUEST_HPP
#define A_REQUEST_HPP

#include "includes.hpp"
#include "parsing/configParser/HeaderParser.hpp"

class Client;

class A_Request
{
	private:
		std::string 											_path;
		std::string 											_method;
		std::string												_httpVersion;
		std::map<std::string , std::vector < std::string > >	_headers;
		std::string												_request;
		bool													_headerDone;
		
public:
	A_Request()
	{
		_headerDone = false;
	};

	std::string &getPath() 
	{
		return (_path);
	}
	
	std::string &getMethod() 
	{
		return (_method);
	}
	
	void parseRequestHeader(std::string request)
	{
		//std::cout << request << std::endl;
		HeaderParser parser(request);
		std::cout << "request == " << request << std::endl;
		_method = parser.getNextToken();
		_path = parser.getNextToken();
		_httpVersion =  parser.getNextToken();
		// std::cout << "method = " << _method << std::endl;
		// std::cout << "_path = " << _path << std::endl;
		// std::cout << "httpVersion = " << _httpVersion << std::endl;

		while (!parser.isDoneParsing())
		{
			std::string requestHeader = parser.getNextToken();
			if (!requestHeader.empty())
			{
				 std::vector < std::string >  values = parser.getValuesCurrToken();
				_headers[requestHeader] = values;
				//std::cout  << requestHeader << " : " << value << std::endl;
			}
			
		}
	}

	std::string &getHttpVersion()
	{
		return (_httpVersion);
	}

	bool isHeaderDone() const
	{
		return (_headerDone);
	}

	virtual void handleRequest(std::string &body, Client &client) = 0;
	
	virtual ~A_Request()
	{

	};
};


#endif