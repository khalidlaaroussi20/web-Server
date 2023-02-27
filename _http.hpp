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




    void getRequest(int Client_Number, ServerMap& Servers)
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
			if (client.clientInfos._requestHandler == nullptr)
				client.factoryRequestHandlerSetter();
			// parse request
			client.clientInfos._requestHandler->parseRequestHeader(header);
			//client.set_response_code(CREATED);
			if (client.clientInfos._requestHandler->isErrorOccured())
			{
				client.set_response_code(BAD_REQUEST);
				client.finished_body();
			}
			else //parsing done successfuly)
			{
				std::string &path = client.clientInfos._requestHandler->getPath();
				if (!client.clientInfos._requestHandler->isValidPath())
				{
					client.set_response_code(BAD_REQUEST);
					client.finished_body();
				}
				else // set server config
				{
					client.setServerConfigs(Servers);
					client.path = path;
					client.setBestLocationMatched();
					client.requestHeaderDone = true;
					if (client.clientInfos._requestHandler->isRequestWellFormed(client) == false)
						client.finished_body();	
				}
			}
			std::cout << "*****Here Check forCGi path == " + client.path + "\n\n";
			client.setPathRessource();// Setting path ressource
			if (client.sendError == false)
				client.setIsRequestForCgi();
			client.factoryResponseHandlerSetter();
		}
		// cgi for get request  i don't know if i should add indexes 
		if (client.sendError)
			client.finished_body();
		else if (client.clientInfos._requestHandler && !client.body_is_done())	//handle request
		{
			client.clientInfos._requestHandler->handleRequest(body, sz, client);
			if(client.body_is_done())
			{
				if (client.isForCgi)
					client.cgiHandler.prepare(&(client.clientInfos));
				std::cout << "Body Done\n";
			}
		}
    };



    void sendResponse(int Client_Number)
    {
		Client &client = _clients[Client_Number];
		A_Response *response =  client.clientInfos._responseHandler;
        if (response->isHeaderSend() == false)
		{
            bool isHeaderSendSuccefuly =  response->sendHeaderResponse(client, _reads, _writes, Client_Number);
			if (!isHeaderSendSuccefuly)
				_clients.dropClient(Client_Number, _reads, _writes);
		}
		else
		{
			response->serve_resource(client);
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