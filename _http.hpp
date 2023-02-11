#ifndef HTTP__HPP
#define HTTP__HPP

#include "includes.hpp"
#include "static/StatusCode.hpp"
#include "server.hpp"
#include "parsing/configParser/ConfigParser.hpp"

namespace ft
{


struct Http{
    fd_set& 		_reads;
    fd_set& 		_writes;
    ListClients& 	_clients;
    Server& 		_server;
    Http(fd_set& reads, fd_set& writes, ListClients& clients, Server& server) : _reads(reads),\
            _writes(writes), _clients(clients), _server(server)
    {};

    void getRequest(int Client_Number)
    {
		Client &client = _clients[Client_Number];
		memset(client.request, 0 , BUFFER_SIZE);
		int sz = recv(client.socket, client.request , BUFFER_SIZE, 0);
		if (sz >= 0)
			client.request[sz] = '\0';
		std::string body = client.request;
		if  (!client.isRequestHeaderDone())
		{
			if (sz < 1)
			{
				printf("Unexpected disconnect from %d.\n",
					client.socket);
				_clients.dropClient(Client_Number, _reads, _writes);
			}
			else
			{
				client.received += sz;
				client.request[client.received] = '\0';
				size_t pos = body.find("\r\n\r\n");
				if (pos == std::string::npos)
				{
					client.set_error_code(BAD_REQUEST);
					return ;
				}
				std::string header = body.substr(0, pos);
				body =  body.substr(std::min(pos + 4,  body.length()) , body.length());
				if (client.requestHandler == nullptr)
					client.factoryRequestHandlerSetter();
				//parse request
				client.requestHandler->parseRequestHeader(header);
				client.requestHeaderDone = true;
			};
		}
		if (client.requestHandler)
		{
			client.requestHandler->handleRequest(body, client);
			if (sz <= 0)
			{
				std::string &path = client.requestHandler->getPath();
				client.path  = new char[path.length() + 1];
				strcpy(client.path, path.c_str());
			}
		}
		

    };



    void sendResponse(int Client_Number)
    {
		Client &client = _clients[Client_Number];
		bool isHeaderSendSuccefuly = true;
        if (client.fp == nullptr)
		{
            isHeaderSendSuccefuly = _server.sendHeaderResponse(client, _reads, _writes, Client_Number);
		}
		if (isHeaderSendSuccefuly)
		{
       		_server.serve_resource(client);
        	if (client.fp == nullptr)
            	_clients.dropClient(Client_Number, _reads, _writes);
		}
    };

    private :


        void get_request_done(int Client_Number)
        {
			Client &client = _clients[Client_Number];
            if (strncmp("GET /", client.request, 5))
				client.set_error_code(BAD_REQUEST);
            else
            {
                char *path = client.request + 4;
                char *end_path = strstr(path, " ");
                if (!end_path)
                    client.set_error_code(BAD_REQUEST);
                else
                {
                    *end_path = 0;
                    client.path = path;
                }
            }
        }
};

}
#endif 