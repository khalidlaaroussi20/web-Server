/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:42:18 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/21 15:52:14 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteRequest.hpp"

void DeleteRequest::handleRequest(std::string &body, size_t size, Client &client)
{
	if (FileSystem::removeAll(client.path.c_str()))
	{
		std::cout << "deleted succes!!\n";
		client.set_response_code(NO_CONTENT);
	}
	else
	{
		std::cout << "deleted fail!!\n";
		client.set_response_code(FORBIDDEN);
	}
	client.finished_body();
}

DeleteRequest::~DeleteRequest()
{

}