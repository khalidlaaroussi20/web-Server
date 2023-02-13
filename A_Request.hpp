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
#include "parsing/requestParser/HeaderParser.hpp"

class Client;

class A_Request
{
	protected:
		std::string 											_path;
		std::string 											_method;
		std::string												_httpVersion;
		std::map<std::string , std::vector < std::string > >	_headers;
		bool													_isErrorOccurs;
		
public:
	typedef std::map<std::string , std::vector < std::string > > headersType;
	A_Request()
	{
		_isErrorOccurs = false;
	};

	std::string &getPath() 
	{
		return (_path);
	}
	
	std::string &getMethod() 
	{
		return (_method);
	}
	
	void parseRequestHeader(std::string &request)
	{
		std::string keysDelimeters = ":";
		std::string valuesDelimeters = ";,=:";
		std::cout << request << std::endl;
		HeaderParser parser(request);
		//std::cout << "request == " << request << std::endl;
		_method = parser.getNextToken(keysDelimeters);
		_path = parser.getNextToken(keysDelimeters);
		_httpVersion =  parser.getNextToken(keysDelimeters);
		std::cout << "method = " << _method << std::endl;
		std::cout << "_path = " << _path << std::endl;
		std::cout << "httpVersion = " << _httpVersion << std::endl;

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
		for (auto xs : _headers)
		{
			std::cout << "values : " << xs.first << " ";
			for (int i = 0; i < xs.second.size(); i++)
				std::cout << xs.second[i] << " ";
			std::cout << std::endl;
		}
		exit(2);
	}

	std::string &getHttpVersion()
	{
		return (_httpVersion);
	}

	headersType &getHeaders()
	{
		return (_headers);
	}

	virtual void handleRequest(std::string &body, size_t size,Client &client) = 0;
	
	virtual ~A_Request()
	{

	};
};


#endif