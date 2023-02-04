/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfigs.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:17:55 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/03 18:47:49 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGS_HPP
#define SERVER_CONFIGS_HPP


#include "Location.hpp"

class ServerConfigs
{
	private :
		std::string 				_listen;
		std::string					_host;
		std::string					_serv;
		std::string 				_serverName;
		unsigned int				_maxClientBodySize;
		std::map<int , std::string> _errorPages;
		std::vector <Location> 		_locations;
	public :
		ServerConfigs(){
			_locations = std::vector <Location>();
			_serv = "80";
		};
		
		//getters
		std::string &getListen()
		{
			return (_listen);
		}

		unsigned int &getMaxClientBodySize()
		{
			return (_maxClientBodySize);
		}

		std::string getHost()
		{
			return (_host);
		}

		std::string getServ()
		{
			return (_serv);
		}

		std::string &getServerName()
		{
			return (_serverName);
		}

		std::map<int , std::string> &getErrorPages()
		{
			return (_errorPages);
		}

		std::vector <Location> &getLocations()
		{
			return (_locations);
		}

		//setters

		void setListen(std::string listen)
		{
			_listen = listen;
			int i = 0;
			std::string tmpHost, tmpPort;
			while (i < _listen.length() && _listen[i] != ':')
				tmpHost += _listen[i++];
			i++;
			while (i < _listen.length())
				tmpPort += _listen[i++];
			setHostAndPort(tmpHost, tmpPort);
		}

		void setHostAndPort(std::string host, std::string port)
		{
			struct addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			struct addrinfo *peer_address;
			if (getaddrinfo(host.c_str(), port.c_str(), &hints, &peer_address)) {
				fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
				exit (FAILURE);
			}

			char buffHost[100];
			char buffServ[100];
			getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
					buffHost, sizeof(buffHost),
					buffServ, sizeof(buffServ),
					NI_NUMERICHOST);
			_host = buffHost;
			_serv = buffServ;
		}

		void setServerName(std::string serverName)
		{
			_serverName = serverName;
		}

		void setMaxClientBodySize(unsigned int maxClientBodySize)
		{
			_maxClientBodySize = maxClientBodySize;
		}

		void setErrorPages(std::map<int , std::string> errorPages)
		{
			_errorPages = errorPages;
		}

		void setLocations(std::vector <Location> locations)
		{
			_locations = locations;
		}

		bool isValidConfigs() const
		{
			if (_host.empty())
				return (false);
			for (int i = 0; i < _locations.size(); i++)
			{
				if (!(_locations[i].isLocationValid()))
					return (false);
			}
			return (true);
		}

		void addErrorPage(int errorCode, std::string path)
		{
			_errorPages[errorCode] = path;
		}
		
};


#endif