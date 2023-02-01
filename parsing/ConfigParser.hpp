/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:56:24 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/01 14:31:11 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes.hpp"

#define OPEN_BRACKET "{"
#define CLOSE_BRACKET "}"


class ConfigParser
{
	private :
		unsigned int currPos;
		std::string serverConfig;
	
	public :
		ConfigParser()
		{
			currPos = 0;
		};
		ConfigParser(std::string serverConfig)
		{
			this ->serverConfig = serverConfig;
			currPos = 0;
		}
		

		std::string getNextToken()
		{
			std::string nextToken = "";
			int  i;
			for (;currPos < serverConfig.length(); currPos++)
			{
				if (isspace(serverConfig[currPos]) || serverConfig[i] == '{' || serverConfig[i] == '}')
				{
					if (!nextToken.empty())
						break;
					else if (serverConfig[i] == '{' || serverConfig[i] == '}')
					{
						nextToken += serverConfig[i];
						currPos;
						break;
					}
					currPos++;
				}
				else
					nextToken += serverConfig[i];
			}
			return (nextToken);
		};

		void skipSpaces()
		{
			while (currPos < serverConfig.length() && serverConfig[currPos] != '\n' && isspace(serverConfig[currPos]))
				currPos++;
		}

		std::vector <std::string> getValuesCurrToken()
		{
			std::vector <std::string> values;
			while (currPos < serverConfig.length() && serverConfig[currPos] != '\n')
			{
				skipSpaces();
				std::string nextToken = getNextToken();
				if (nextToken == "{" || nextToken == "}")
				{
					currPos--;
					break;
				}
				values.push_back(nextToken);
			}
			return (values);
		};
		
		void pError(char *msg)
		{
			std::cerr << msg << std::endl;
			exit (FAILURE);
		}

		void parseServerConfig()
		{
			std::stack <std::string> bracketStack;
			while (currPos < serverConfig.length())
			{
				std::string nextToken = getNextToken();
				if (bracketStack.empty())
				{
					if (nextToken == "server")
					{
						nextToken = getNextToken();
						if (nextToken != OPEN_BRACKET)
							pError("Parsing Error");
						bracketStack.push(OPEN_BRACKET);
					}
				}
				else
				{
					
				}
			}
		}
}

#endif
