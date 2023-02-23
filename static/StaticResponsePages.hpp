/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticResponsePages.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:13:53 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/23 14:37:22 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATIC_RESPONSE_PAGES
#define STATIC_RESPONSE_PAGES



#include "../includes.hpp"
#include "./StatusCode.hpp"
#define PATH_404 "./static/response_pages/404.html"
#define PATH_400 "./static/response_pages/400.html"
#define PATH_403 "./static/response_pages/403.html"
#define PATH_405 "./static/response_pages/405.html"
#define PATH_413 "./static/response_pages/413.html"
#define PATH_414 "./static/response_pages/414.html"
#define PATH_494 "./static/response_pages/494.html"
#define PATH_500 "./static/response_pages/500.html"
#define PATH_501 "./static/response_pages/501.html"
#define PATH_200 "./static/response_pages/200.html"
#define PATH_201 "./static/response_pages/201.html"
#define PATH_204 "./static/response_pages/204.html"
#define PATH_301 "./static/response_pages/301.html"




struct StaticResponsePages
{
	static  std::map < int  , std::string>  RESPONSE_PAGES;
	static 	std::map < int  , std::string> S_InitResponsePages()
	{
		std::map < int  , std::string> responsePages;
		responsePages[NOT_FOUND] = PATH_404;
		responsePages[BAD_REQUEST] = PATH_400;
		responsePages[FORBIDDEN] = PATH_403;
		responsePages[METHOD_NOT_ALLOWED] = PATH_405;
		responsePages[REQUEST_URI_TOO_LONG] = PATH_414;
		responsePages[REQUEST_ENTITY_TOO_LARGE] = PATH_413;
		responsePages[REQUEST_HEADER_TOO_LARGE] = PATH_494;


		
		responsePages[INTERNAL_SERVER_ERROR] = PATH_500;
		responsePages[NOT_IMPLEMENTED] = PATH_501;

		
		responsePages[OK] = PATH_200;
		responsePages[CREATED] = PATH_201;
		responsePages[NO_CONTENT] = PATH_204;

		responsePages[MOVED_PERMANETLY] = PATH_301;
		
		return responsePages;
	}
};



#endif