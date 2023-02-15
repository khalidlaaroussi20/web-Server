/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:48:36 by mel-amma          #+#    #+#             */
/*   Updated: 2023/02/15 15:43:46 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostRequest.hpp"
#include "ChunckContentHandler.hpp"

PostRequest::PostRequest()
{
    file_initialized = false;
    is_chunked = false;
};

bool PostRequest::post_init()
{
    auto it = _headers.find("Content-Length");
    if (it != _headers.end())
    {
        std::vector<std::string> content_type_vector = it->second;
        std::string content_len = content_type_vector[0];
        body_length = std::stoi(content_len);
    }
    else
    {
        body_length = 0;
        is_chunked = true;
    }
    received = 0;
    file_initialized = false;
    if(is_chunked)
    {
        auto it = _headers.find("Transfer-Encoding");
        if (it == _headers.end() || it->second[0] != "chunked")
            return false;
    }
    return (true);
}


/*  send in the buffer itself or its address and the size to write  */
void PostRequest::handleRequest(std::string &body, size_t size, Client &client)
{
	//size is what received read
    auto it = _headers.find("Content-Type");
    if (it == _headers.end())
    {
        // case no body
        client.finished_body();
        return;
	}

	std::vector<std::string> content_type_vector = it->second;
    std::string content_type = content_type_vector[0];

    if (!file_initialized)
    {  
        if(!post_init())
        {
            client.set_response_code(BAD_REQUEST);
            client.finished_body();
            return ;
        }
    }

    std::vector<const char *>  chunks;
    if(is_chunked)
    {
        // std::cout << body;
        if(!chunk_handler.getHttpChunkContent(body.c_str(),size,chunks))
        {
            std::cout << "chunks parsing failed\n";
            client.set_response_code(BAD_REQUEST);
            client.finished_body();
            return ;
        }
        std::cout << "chunk size = " << chunks.size() << std::endl;
        for (int i = 0; i < chunks.size(); i +=2)
        {
            const char *a =  chunks[i];
            std::string currChunk;
            while (a !=  chunks[i + 1])
            {
                currChunk += *a;
                a++;
            }
            std::cout << "chunk = " << currChunk << std::endl;
        }
    }
    /*
        if chunked then clean it first then forward to the next ifs
        update size after cleaning the body of chunk (just clean of chunk and its size and send it in even if its half a reques
        just as if you were gonna send a normal request, chunk will help us to only define when to stop (when its 0))
        check if chunk size is in midde of the request (in case he sends two chuncks and you recv 1 and a half)
        if chunk size is 0 meaning its done then  close and flag up its done with body
    */

    if (content_type == "multipart/form-data")
    {
        // take care content-type or content_disposition with their boundaries having boundary set somewhere

        // fs.close(); then make a new file when theres a new one
    }
    else
    {
        if(!file_initialized)
        {
            std::string str("./public/uploads/upload_");
	        std::cout << "created File\n";
	        // upload_store check if its there, check upload pass or just put in default upload path
	        fs = FileSystem(str /*get best match*/, WRITE, ContentTypes::getExtention(content_type));
	        fs.open();
            file_initialized = true;
        }
        //maybe clean of \n\r and such?
        is_chunked? write_body(chunks,size): write_body(body,size) ;
    }
    if (!is_chunked && body_length <= received)
    {
        fs.close();
        setBodyAsFinished(client);
    }
    else if (is_chunked && chunk_handler.is_done())
    {
        fs.close();
        setBodyAsFinished(client);
    }
};


void PostRequest::write_body(std::string& body, size_t size)
{
    received += size;
    fs.Write_chunk(body,size);
}

void PostRequest::write_body(std::vector<const char *>  &chunks, size_t size)
{
    for (int i = 0; i < chunks.size(); i +=2)
    {
        const char *a =  chunks[i];
        int _size = chunks[i + 1] - chunks[i];
        fs.Write_chunk(a,_size);
    }
};


PostRequest::~PostRequest()
{
}

void PostRequest::setBodyAsFinished(Client &client)
{
	client.finished_body();
	client.set_response_code(CREATED);
}