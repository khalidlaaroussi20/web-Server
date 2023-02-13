/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-amma <mel-amma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:42:18 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/12 18:16:05 by mel-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetRequest.hpp"
#include "client.hpp"

void GetRequest::handleRequest(std::string &body, size_t size, Client &client)
{
	if (!body.empty())
	{
		client.set_error_code(BAD_REQUEST);
	}
	client.finished_body();
}

GetRequest::~GetRequest()
{

}