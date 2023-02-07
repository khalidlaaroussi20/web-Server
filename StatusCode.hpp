/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:19:04 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/07 11:52:33 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_CODE_HPP
#define STATUS_CODE_HPP


enum StatusCode
{
	NOT_FOUND = 404,
	BAD_REQUEST = 400,
	INTERNAL_SERVER_ERROR = 500,
	OK = 200
};


#endif