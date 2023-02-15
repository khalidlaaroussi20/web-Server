/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   staticResponseMessages.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:26:09 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/15 12:45:41 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATIC_RESPONSE_MESSSAGES_HPP
#define STATIC_RESPONSE_MESSSAGES_HPP

#include "../includes.hpp"
#include "StatusCode.hpp"

class StaticResponseMessages
{
	public :
		static std::map<int, std::string> MAPPING_RESPONSE_CODE_TO_MESSAGES;

		static  std::map<int, std::string> S_initResponseMessages()
		{
			std::map<int, std::string> Messages;
			Messages[OK] = " OK ";
			Messages[CREATED] = " Created ";
			
			Messages[BAD_REQUEST] = " Bad Request ";
			Messages[NOT_FOUND] = " Not Found ";
			Messages[BAD_REQUEST] = " Bad Request ";
			Messages[METHOD_NOT_ALLOWED] = " Method Not Allowed ";
			Messages[FORBIDDEN] = " Forbidden ";
			Messages[REQUEST_ENTITY_TOO_LARGE] = " Request Entity Too Large ";
			Messages[REQUEST_HEADER_TOO_LARGE] = " Request Header Too Large ";
			
			
			Messages[INTERNAL_SERVER_ERROR] = " Internal Server Error ";
			Messages[NOT_IMPLEMENTED] = " Not Implemented ";

			Messages[MOVED_PERMANETLY] = " Moved Permanetly ";

			return (Messages);
		}

		static  std::string &getMessageResponseCode(int responseCode)
		{
			return (MAPPING_RESPONSE_CODE_TO_MESSAGES[responseCode]);
		}
};



#endif
