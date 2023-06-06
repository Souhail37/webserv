
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   appendClientSide.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fech-cha <fech-cha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 20:05:34 by fech-cha          #+#    #+#             */
/*   Updated: 2023/05/31 05:15:44 by fech-cha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "pollingServ.hpp"


appendClient::appendClient(): _checkHead(0), _checkBody(0), _clientFd(-69), _responseStatus(0), _responseSent(0)
{
    
}

appendClient::~appendClient()
{
    
}
 
void    appendClient::setClientFd(int fd)
{
    this->_clientFd = fd;
}

int appendClient::getClientFd(void)
{
    return (this->_clientFd);
}

int appendClient::getHeadStatus()
{
    return (this->_checkHead);
}

void    appendClient::setHeadStatus(int head)
{
    this->_checkHead = head;
}

int appendClient::getBodyStatus()
{
    return (this->_checkBody);
}

void    appendClient::setBodyStatus(int Body)
{
    this->_checkBody = Body;
}

std::string appendClient::getHeader()
{
    return (this->_header);
}

std::string appendClient::getBody()
{
    return (this->_body);
}

void    appendClient::setBody(std::string body)
{
    for(int i = 0; i < body.size(); i++)
        this->_body.push_back(body[i]);
}

int appendClient::getResponseStat()
{
    return (_responseStatus);
}

void appendClient::setResponseStat(int stat)
{
    this->_responseStatus = stat;
}

std::string appendClient::getRestOfRes(int size)
{

}

void    appendClient::sendReq(int sockfd)
{
    int check;
    check = send(sockfd, getResponse().c_str(), getResponse.size(), 0);
    if (check < 0)
    {
        perror("send");
        // error
    }
    if (this->response.size() > 0)
        this->response.erase(0, check);
    if (this->response.size() == 0)
        this->setResponseStat(closeConnect);
    // if (this->_responseStatus == closeConnect)
        //close connection
}

void    appendClient::copyReq(char *req, int size)
{
    int i = 0;
    if (!this->_tmp.empty())
        this->_tmp.erase();
    while (i < size)
    {
        this->_tmp.push_back(req[i]);
        i++;
    }
}

std::string::size_type appendClient::checkCRLForChunk(std::string test)
{
    if (test == CRLF)
    {
        std::string::size_type pos = this->_header.find(test);
        if (pos != std::string::npos)
            return (pos);
    }
    if (test == lastChunk)
    {
        std::string::size_type pos = this->_body.find(test);
        if (pos != std::string::npos)
            return (pos);
    }
    return (-1);
}

void    appendClient::getBodyRest()
{
    //check pos of CRLF and get the beginning of the body
    std::string::size_type pos = this->_header.find(CRLF);

    if (pos != std::string::npos)
    {
        //extract body
        std::string res = this->_header.substr(pos + 4);
        for (std::string::size_type i = pos + 4; pos < this->_header.size(); i++)
            this->_body.push_back(this->_header[i]);
        this->_header.erase(pos + 4);
    }
}

void    appendClient::recvBody(std::string req)
{
    this->setBody(req);
    if (this->_bodyType == contentLength)
    {
        if (sizeOfContentLength > 0)
            sizeOfContentLength -= this->_body.size();
        else
            this->_responseStatus = responseGo;
    }
    else if (this->_bodyType == chunked)
    {
        if (this->checkCRLForChunk(lastChunk))
        {
            this->parseChunked(this->_body);
            this->_responseStatus = responseGo;
        }
    } 
}

void    appendClient::getBodyType()
{
    std::string::size_type content_length = this->_header.find("Content-Length");
    std::string::size_type transfer_chunked = this->_header.find("Transfer-Encoding: chunked");
    if (content_length != std::string::npos) {
        this->_bodyType = contentLength;
    } else if (transfer_chunked != std::string::npos){
        this->_bodyType = chunked;
    }
}

void    appendClient::setHTTPRequest()
{
    this->_httpRequest.append(this->_header);
    for (int i = 0; i < this->_body.size(); i++)
        this->_httpRequest.push_back(this->_body[i]);
}

void    appendClient::recvHead()
{
    int check;
    char tmp[BUFFER_SIZE];
    check = recv(this->_clientFd, tmp, BUFFER_SIZE, 0);
    if (check < 0)
    {
        perror("recv");
        //exit or return 
    }
    appendClient::copyReq(tmp, BUFFER_SIZE);
    if (this->getHeadStatus() == endOfHeader)
    {
        appendClient::recvBody(this->_tmp);
    }
    else if (this->getHeadStatus() != endOfHeader)
    {
        this->_header.append(this->_tmp);

        //check for CRLF at the end of the string
        if (this->checkCRLForChunk(CRLF) >= 0)
            {
                this->setHeadStatus(endOfHeader);
                //check this
                this->getBodyRest();
                //look for body type
                this->getBodyType();
                if (this->_bodyType == chunked && this->checkCRLForChunk(lastChunk))
            {
                this->parseChunked(this->_body);
                if (this->_bodyType == endOfBody)
                {
                    this->setHTTPRequest();
                    this->_responseStatus = responseGo;
                }
            }
            }
        // else //error cases
        // {
        //     //exit or return or throw exception
        // }
    }
}

void    appendClient::parseChunked(std::string& chunkedData)
{
     int chunkflag = 0;
    std::istringstream iss(chunkedData);
    std::string res;

    std::string line;
    std::size_t chunkSize;
    std::string chunk = "";

    // Read and process each chunk
    while (std::getline(iss, line)) {
        if (chunkflag == 0)
        {
        std::istringstream chunkSizeStream(line);
        if (!(chunkSizeStream >> std::hex >> chunkSize)) {
            std::cerr << "Error: Invalid chunk size." << std::endl;
            break ;
        }
        else
        {
            chunkflag = 1;
            continue;
        }
        }

        if (chunkSize == 0) {
            break;  // Reached the end of the chunked data
        }

        if (chunkflag == 1)
        {   
            std::stringstream chunkStream(line);
            chunkStream >> chunk;
            if (chunk.size() != chunkSize)
            {
                std::cout << "Error size doesn't match" << std::endl;
                break;
            }
            else
            {
                res.append(chunk);
                chunk = "";
            }
            chunkflag = 0;
        }
    }
    this->_body.clear();
    this->_body.append(res);
    this->_checkBody = endOfBody;
}
