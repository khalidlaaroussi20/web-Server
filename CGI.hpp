/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 15:58:42 by klaarous          #+#    #+#             */
/*   Updated: 2023/02/25 17:45:41 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/***************************************************/
/*     created by TheWebServerTeam 2/24/23         */
/***************************************************/

#ifndef DUMMY_CLIENT_CGI_H
#define DUMMY_CLIENT_CGI_H


#include "includes.hpp"


class Client;



class CGI
{
	public:
		enum {
			NONE = 0, DONE = 1, ERROR = -1
		};
	public:
		typedef std::map<std::string, std::string> meta_var_type;
	private:
		Client			_client;
		int 			_status;
		meta_var_type	_metaVars;
		int 			_CGIPid;
		int 			_readEnd;
		int 			_bodyFd;
		std::string 	_outFilePath;
		int				_outFileFd;
	private:
		CGI(const CGI& other);
		CGI& operator=(const CGI& other);
		
	public:

		CGI();

		~CGI();

	public:

		int getStatus() const;
		int getCgiPid() const;

		const std::string &getOutFilePath() const;

	public:

		static std::string toEnvHeader(const std::string& header);

		std::string _generateRandomPath();

		std::string removeDotDot(std::string path);
		
		void prepare(Client& client);

		void read();
	private:

		void _execCGI(char **args, char **env, int bodyFd);

		void _execCGI();

};


#endif //DUMMY_CLIENT_CGI_H
