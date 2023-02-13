/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 17:48:36 by mel-amma          #+#    #+#             */
/*   Updated: 2023/02/13 17:34:37 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostRequest.hpp"

PostRequest::PostRequest()
{
    file_initialized = false;
    is_chunked = false;
};

void PostRequest::post_init(std::string &contentType)
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
	std::string str("./public/uploads/upload_");
	std::cout << "created File\n";
	// upload_store check if its there, check upload pass or just put in default upload path
	fs = FileSystem(str /*get best match*/, WRITE, ContentTypes::getExtention(contentType));
	fs.open();
    file_initialized = true;
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

	std::vector<std::string> content_type_vector = _headers.at("Content-Type");
    std::string content_type = content_type_vector[0];

    if (!file_initialized)
        post_init(content_type);

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
        //maybe clean of \n\r and such?
        received += size;
        fs.Write_chunk(body,size);
    }

    if (body_length <= received)
    {
        fs.close();
        client.finished_body();
    }
};

PostRequest::~PostRequest()
{
}