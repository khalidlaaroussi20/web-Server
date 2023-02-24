/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 12:00:05 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/24 14:01:21 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_RESPONSE_HPP
#define CGI_RESPONSE_HPP

#include "A_Response.hpp"
#include "client.hpp"
#include "parsing/requestParser/HeaderParser.hpp"


class CGIResponse : public  A_Response
{
	private :
		std::string										_header;
		std::string										_body;
		std::map <std::string, std::vector <std::string > > 	_cgiHeaders;
		void 											_parseCgiHeader(std::string buffer ,Client &client);
		void											_setBody(std::string body);
	public :
		CGIResponse();
		std::string getHeaderResponse(Client &client);
};


#endif
