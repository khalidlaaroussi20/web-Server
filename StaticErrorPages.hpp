/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticErrorPages.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:13:53 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/07 11:35:07 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATIC_ERROR_PAGES
#define STATIC_ERROR_PAGES



#include "./includes.hpp"
#include "./StatusCode.hpp"
#define PATH_404 "./public/errors/404.html"
#define PATH_400 "./public/errors/400.html"


struct StaticErrorPages
{
	static  std::map < int  , std::string>  ERROR_PAGES;
	static 	std::map < int  , std::string> S_InitErrorPages()
	{
		std::map < int  , std::string> errorPages;
		errorPages[NOT_FOUND] = PATH_404;
		errorPages[BAD_REQUEST] = PATH_400;
		return errorPages;
	}
};

std::map < int  , std::string> StaticErrorPages::ERROR_PAGES = StaticErrorPages::S_InitErrorPages();



#endif