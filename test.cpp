/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 14:50:40 by mel-amma          #+#    #+#             */
/*   Updated: 2023/02/19 16:25:37 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "GetRequest.hpp"

int main()
{
	char buffer[3];

	buffer[0] = 0xd9;
	buffer[1] = 0x81;
	buffer[2] = 0;
	std::string a = buffer;
	std::ofstream file(a);
	// int fd = open(buffer, O_CREAT);
	// printf("%d\n", fd);
	// GetRequest getRequest;

	// std::string request = ""
    return (0);
}