/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:56:24 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/22 17:53:32 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_PARSER_HPP
#define HEADER_PARSER_HPP

#include "../../includes.hpp"




class HeaderParser
{
	private :
		unsigned int 	_currPos;
		std::string 	_request;
	
	public :
		HeaderParser()
		{
			_currPos = 0;
		};
		
		HeaderParser(std::string _request)
		{
			this ->_request = _request;
			_currPos = 0;
		}

		bool	isDoneParsing() const
		{
			return (_currPos >= _request.length());
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
			while (_currPos < _request.length())
			{
				bool isDel = isDelimeter(_request[_currPos], delimeter);
				if ((_currPos + 1 < _request.length() && _request[_currPos] == '\r' && _request[_currPos + 1] == '\n') \
					|| isspace(_request[_currPos]) || isDel)
				{
					if (nextToken.empty() &&  (isDel))
					{
						nextToken += _request[_currPos];
						_currPos++;
						break;
					}
					if (!nextToken.empty())
						break;
				}
				else
					nextToken += _request[_currPos];
				_currPos++;
			}
			return (nextToken);
		};

		

		std::string getHeaderValue()
		{
			std::string nextToken = "";
			while (_currPos < _request.length())
			{
				if (_request[_currPos] == ':')
				{
					_currPos++;
					skipSpaces();
				}
				if (_request[_currPos] == '\n')
					break;
				if ((_currPos + 1 < _request.length() && _request[_currPos] == '\r' && _request[_currPos + 1] == '\n'))
				{
					_currPos++;
					if (!nextToken.empty())
						break;
				}
				else
					nextToken += _request[_currPos];
				_currPos++;
			}
			_currPos++;
			return (nextToken);
		}

		void pErrorParsing(const char *msg) const
		{
			std::cerr << msg << std::endl;
			exit (FAILURE);
		}

		void skipSpaces()
		{
			while (_currPos < _request.length() && _request[_currPos] != '\n' && isspace(_request[_currPos]))
				_currPos++;
		}


		std::vector <std::string> getValuesCurrToken(std::string &delimeters, bool &isErrorOccurs)
		{
			std::vector <std::string> values;
			skipSpaces();
			std::string nextToken = getNextToken(delimeters);
			if (nextToken == ":")
			{
				while (_currPos < _request.length() && _request[_currPos] != '\n')
				{
					skipSpaces();
					if (_request[_currPos] == '\n')
						break;
					nextToken = getNextToken(delimeters);
					if (nextToken.size() > 1 || !isDelimeter(nextToken[0] ,delimeters))
						values.push_back(nextToken);
				}
			}
			else
				isErrorOccurs = true;
			return (values);
		};

};

#endif
