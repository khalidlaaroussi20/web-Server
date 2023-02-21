/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:42:18 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/20 18:40:48 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

void CgiHandler::handleRequest(std::string &body, size_t size, Client &client)
{
	client.finished_body();
}

CgiHandler::~CgiHandler()
{

}