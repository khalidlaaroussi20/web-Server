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
#include "static/StatusCode.hpp"
#include "parsing/configParser/Location.hpp"
#include "parsing/requestParser/HeaderParser.hpp"
#include "HeaderPath.hpp"

class Client;

class A_Request
{
	protected:
		std::string 											_path;
		std::string												_query;
		std::string 											_method;
		std::string												_httpVersion;
		std::map<std::string , std::vector < std::string > >	_headers;
		bool													_isErrorOccurs;
		std::map<std::string , std::string >					_headersForCgi;
		
public:
	typedef std::map<std::string , std::vector < std::string > > headersType;
	A_Request();
	std::string &getPath();

	std::string &getQuery();
	
	std::string &getMethod();

	
	void parseRequestHeader(std::string &request);
	void parsePath(HeaderParser &parser);

	void setHeadersRequest(std::string &request);
	void setHeadersForCgi(std::string &request);

	std::string &getHttpVersion();

	headersType &getHeaders();

	std::string getHeaderValue(const std::string header);

	bool isRequestWellFormed(Client &client);


	virtual void handleRequest(std::string &body, size_t size, Client &client) = 0;

	bool isValidPath();

	bool isErrorOccured() const;

	std::string getPathRessource(Location &bestLocationMatched);

	void addHeaderToCgi(std::string headerKey, std::string headerValue);

	std::string getHeaderCgiValue(std::string header);
	
	void printCgisHeaders()
	{
		std::cout << "headers for cgis : \n\n";
		for (auto xs : _headersForCgi)
		{
			std::cout << xs.first << "=" << xs.second << std::endl;
		}
	}

	void setBodyAsFinished(Client &client);
	
	virtual ~A_Request()
	{

	};
};


#endif