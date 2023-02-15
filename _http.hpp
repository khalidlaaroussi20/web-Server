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

    void getRequest(int Client_Number, ServerMap& SameSocketServers)
    {
		// std::cout <<"getRequest\n";
		Client &client = _clients[Client_Number];
		memset(client.request, 0 , BUFFER_SIZE);
		size_t sz = recv(client.socket, client.request , BUFFER_SIZE, 0);
		if (sz < 1)
		{
			printf("Unexpected disconnect from %d.\n",
				client.socket);
			_clients.dropClient(Client_Number, _reads, _writes);
			return ;
		}
		client.request[sz] = 0;		
		std::string body(client.request,sz);
		if  (!client.isRequestHeaderDone())
		{
			client.received += sz;
			client.request[client.received] = '\0';
			size_t pos = body.find("\r\n\r\n");
			if (pos == std::string::npos)
			{
				client.set_response_code(REQUEST_HEADER_TOO_LARGE);
				client.finished_body();
				return ;
			}
			std::string header = body.substr(0, pos);
			body = body.substr(std::min(pos + 4, sz), sz);
			sz = sz - std::min(pos + 4, sz);
			if (client.requestHandler == nullptr)
				client.factoryRequestHandlerSetter();
			// parse request
			client.requestHandler->parseRequestHeader(header);
			// set server config
			// loop through servers and add server configs of the matched one from the request in the client
			ServerConfigs *requestConfigs;
			{
				A_Request::headersType headers = client.requestHandler->getHeaders();
				std::string host = headers.at("Host")[0];

				requestConfigs = &((SameSocketServers.begin())->second.getServerConfigs());
				for (auto &serv : SameSocketServers)
				{
					if (host == serv.first)
					{
						requestConfigs = &(serv.second.getServerConfigs());
						break ;
					}
				}
			}
			client.set_request_configs(requestConfigs);
			std::string &path = client.requestHandler->getPath();
			client.path = new char[path.length() + 1];
			strcpy(client.path, path.c_str());
			client.setBestLocationMatched();
			client.requestHeaderDone = true;
			if (client.requestHandler->isRequestWellFormed(client) == false)
			{
				client.finished_body();
				return ;
			}
		
		}
		if (client.sendError == false && client.requestHandler)
		{
			client.requestHandler->handleRequest(body, sz, client);
			if(client.body_is_done())
				std::cout << "body_done " << std::endl;
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
			//close connection drop client?
				//else if keep alive, reset request and body etc and leave socket and all alive
        	if (client.fp == nullptr)
            	_clients.dropClient(Client_Number, _reads, _writes);
		}
    };

    private :
		void substr_size(std::string& str, size_t pos ,size_t size)
		{
			
		}
};

}
#endif 