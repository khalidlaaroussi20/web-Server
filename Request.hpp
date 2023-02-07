/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:30:21 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/07 15:58:41 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "includes.hpp"
#include "parsing/configParser/ConfigParser.hpp"

class Request
{
	private:
		std::string 						_path;
		std::string 						_method;
		std::string							_httpVersion;
		std::map<std::string , std::string> _headers;
		std::string							_request;
		std::string							_body;
		
public:
	Request();

	void parseRequestHeader()
	{
		ConfigParser parser(_request);
		_method = parser.getNextToken();
		_path = parser.getNextToken();
		_httpVersion =  parser.getNextToken();
		while (!parser.isDoneParsing())
		{
			std::string requestHeader = parser.getNextToken();
		}
		
	}
	
	~Request();
};


#endif