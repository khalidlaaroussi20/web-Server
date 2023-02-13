/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:56:24 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/13 18:03:23 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_PARSER_HPP
#define HEADER_PARSER_HPP

#include "../../includes.hpp"




class HeaderParser
{
	private :
		unsigned int _currPos;
		std::string _serverConfig;
	
	public :
		HeaderParser()
		{
			_currPos = 0;
		};
		
		HeaderParser(std::string _serverConfig)
		{
			this ->_serverConfig = _serverConfig;
			_currPos = 0;
		}

		bool	isDoneParsing() const
		{
			return (_currPos >= _serverConfig.length());
		}
		

		bool isDelimeter(char c, std::string &delimeter)
		{
			for (int i = 0; i < delimeter.length(); i++)
			{
				if (c == delimeter[i])
					return (true);
			}
			return (false);
		}

		std::string getNextToken(std::string &delimeter)
		{
			std::string nextToken = "";
			while (_currPos < _serverConfig.length())
			{
				bool isDel = isDelimeter(_serverConfig[_currPos], delimeter);
				if ((_currPos + 1 < _serverConfig.length() && _serverConfig[_currPos] == '\r' && _serverConfig[_currPos + 1] == '\n') \
					|| isspace(_serverConfig[_currPos]) || isDel)
				{
					if (nextToken.empty() &&  (isDel))
					{
						nextToken += _serverConfig[_currPos];
						_currPos++;
						break;
					}
					if (!nextToken.empty())
						break;
				}
				else
					nextToken += _serverConfig[_currPos];
				_currPos++;
			}
			return (nextToken);
		};

		void pErrorParsing(const char *msg) const
		{
			std::cerr << msg << std::endl;
			exit (FAILURE);
		}

		void skipSpaces()
		{
			while (_currPos < _serverConfig.length() && _serverConfig[_currPos] != '\n' && isspace(_serverConfig[_currPos]))
				_currPos++;
		}


		std::vector <std::string> getValuesCurrToken(std::string &delimeters)
		{
			std::vector <std::string> values;
			skipSpaces();
			std::string nextToken = getNextToken(delimeters);
			if (nextToken == ":")
			{
				while (_currPos < _serverConfig.length() && _serverConfig[_currPos] != '\n')
				{
					skipSpaces();
					if (_serverConfig[_currPos] == '\n')
						break;
					nextToken = getNextToken(delimeters);
					if (nextToken.size() > 1 || !isDelimeter(nextToken[0] ,delimeters))
						values.push_back(nextToken);
				}
			}
			return (values);
		};
};

#endif
