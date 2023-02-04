/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 16:56:49 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/03 18:47:14 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"


int main(int ac, char **av)
{
	std::string configServer;
	if (ac != 2)
	{
		std::cerr << "number argument Not valid !" << std::endl;
		return (0);
	}
	std::ifstream myfile (av[1]);
	
	std::string serverConfigs = "";
	if ( myfile.is_open() ) 
	{

		std::string sa;
        while (getline(myfile, sa))
		
            serverConfigs += sa + "\n";
        myfile.close();
	}
	ConfigParser parser = ConfigParser(serverConfigs);
	parser.parseServerConfig();

	return (0);
}