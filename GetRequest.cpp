/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:42:18 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/21 16:53:07 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequest.hpp"

void GetRequest::handleRequest(std::string &body, size_t size, Client &client)
{
	if (!body.empty())
		client.set_response_code(BAD_REQUEST);
	client.tryOpenRessource();
	client.finished_body();
}

GetRequest::~GetRequest()
{

}