/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfos.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:51:10 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/25 19:09:00 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_INFOS_HPP
#define CLIENT_INFOS_HPP

#include "includes.hpp"
#include "static/StatusCode.hpp"
#include "A_Request.hpp"
#include "A_Response.hpp"
#include "parsing/configParser/Location.hpp"
#include "parsing/configParser/ServerConfigs.hpp"

struct ClientInfos
{
	A_Request 				*_requestHandler;
	ServerConfigs			*_serverConfigs;
	Location				*_bestLocationMatched;
	A_Response				*_responseHandler;
	std::string				_cgiPath;
	std::string				_indexPath;
	std::string				_cgiFilePath;
	std::string				_addr;
	std::string				_port;
	

	ClientInfos() {
		_requestHandler = nullptr;
		_responseHandler = nullptr;
		_bestLocationMatched = nullptr;
		_responseHandler = nullptr;
	};
	
	void setRequestHandler(A_Request *req)
	{
		_requestHandler = req;
	}

	void setResponseHandler(A_Response *res)
	{
		_responseHandler = res;
	}

	void setServerConfigs(ServerConfigs *conf)
	{
		_serverConfigs = conf;
	}


	void setBestLocation(Location *location)
	{
		_bestLocationMatched = location;
	}
	

	void setCgiPath(std::string cgiPath)
	{
		_cgiPath = cgiPath;
	}

	void setIndexPath(std::string indexPath)	
	{
		_indexPath = indexPath;
	}

	void setCgiFilePath(std::string cgiFilePath)	
	{
		_cgiFilePath = cgiFilePath;
	}

	~ClientInfos()
	{
		std::cout << "Destruct ClientInfos\n\n";
		// if (_requestHandler)
		// 	delete _requestHandler;
		// if (_responseHandler)
		// 	delete _responseHandler;
		// _requestHandler = nullptr;
		// _responseHandler = nullptr;
	}
};

#endif