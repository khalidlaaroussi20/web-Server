/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticErrorPages.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:13:53 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/15 12:07:06 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATIC_ERROR_PAGES
#define STATIC_ERROR_PAGES



#include "../includes.hpp"
#include "./StatusCode.hpp"
#define PATH_404 "./public/errors/404.html"
#define PATH_400 "./public/errors/400.html"
#define PATH_403 "./public/errors/403.html"
#define PATH_405 "./public/errors/405.html"
#define PATH_413 "./public/errors/413.html"
#define PATH_414 "./public/errors/414.html"
#define PATH_494 "./public/errors/494.html"
#define PATH_500 "./public/errors/500.html"
#define PATH_501 "./public/errors/501.html"
#define PATH_200 "./public/errors/200.html"
#define PATH_201 "./public/errors/201.html"
#define PATH_301 "./public/errors/301.html"




struct StaticErrorPages
{
	static  std::map < int  , std::string>  ERROR_PAGES;
	static 	std::map < int  , std::string> S_InitErrorPages()
	{
		std::map < int  , std::string> errorPages;
		errorPages[NOT_FOUND] = PATH_404;
		errorPages[BAD_REQUEST] = PATH_400;
		errorPages[FORBIDDEN] = PATH_403;
		errorPages[METHOD_NOT_ALLOWED] = PATH_405;
		errorPages[REQUEST_URI_TOO_LONG] = PATH_414;
		errorPages[REQUEST_ENTITY_TOO_LARGE] = PATH_413;
		errorPages[REQUEST_HEADER_TOO_LARGE] = PATH_494;


		
		errorPages[INTERNAL_SERVER_ERROR] = PATH_500;
		errorPages[NOT_IMPLEMENTED] = PATH_501;

		
		errorPages[OK] = PATH_200;
		errorPages[CREATED] = PATH_201;

		errorPages[MOVED_PERMANETLY] = PATH_301;
		
		return errorPages;
	}
};



#endif