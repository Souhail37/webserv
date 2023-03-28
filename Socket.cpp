/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fech-cha <fech-cha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 22:43:07 by fech-cha          #+#    #+#             */
/*   Updated: 2023/03/28 08:01:01 by fech-cha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

mySocket::mySocket()
{
    //define address structure
    memset(&this->hints, 0, sizeof(this->hints)); //empty the struct
    this->hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
    this->hints.ai_socktype = SOCK_STREAM; //TCP sockets
    this->hints.ai_flags = AI_PASSIVE; // indicates that the address is intended for a listening socket,bind to the IP of the host it’s running

    if (int val = getaddrinfo(NULL, HTTP_PORT, &this->hints, &this->servinfo))
    {
        std::string err = gai_strerror(val);
        std::cout << "getaddrinfo error: " << err << std::endl;
        exit(EXIT_FAILURE);
    }
    //create socket
    this->sockfd = socket(this->servinfo->ai_family, this->servinfo->ai_socktype, this->servinfo->ai_protocol);
    mySocket::testSysCall(getSockFd());
    std::cout << "Socket created succesfully." << std::endl;
    
    //bind (though it should be outside the constructor)
    this->bindRes = bind(getSockFd(), this->servinfo->ai_addr, this->servinfo->ai_addrlen);
    mySocket::testSysCall(getBindValue());
    std::cout << "Socket succesfully bound to address." << std::endl;
}

mySocket::~mySocket()
{
    freeaddrinfo(this->servinfo);
}

void    mySocket::testSysCall(int fd)
{
    if (fd < 0)
    {
        perror("Network Failure.");
        exit(EXIT_FAILURE);
    }
}

void    mySocket::rerunServ(void)
{
    int enable = 1;
    
    int check = setsockopt(getSockFd(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    mySocket::testSysCall(check);
}

int mySocket::getSockFd(void) const
{
    return (this->sockfd);    
}

int mySocket::getBindValue(void) const
{
    return (this->bindRes);
}

int mySocket::getAcceptFd(void) const
{
    return (this->acceptSockFd);
}

void    mySocket::listenRequest(void)
{
    int check = listen(getSockFd(), BACKLOG);
    mySocket::testSysCall(check);
    std::cout << "Server listening for connections ... " << std::endl;
}

void    mySocket::acceptConnection(void)
{
    this->incomingAddSize = sizeof(this->incomingStruct);
    this->acceptSockFd = accept(getSockFd(), (struct sockaddr *)&this->incomingStruct, &this->incomingAddSize);
    mySocket::testSysCall(this->acceptSockFd);
}

void    mySocket::closeConnection(void)
{
    close(this->acceptSockFd);
    close(this->sockfd);
}

void    mySocket::sendReq(int sockfd, const void *buf, int len, int flags)
{
    this->sendRes = send(sockfd, buf, len, flags);
    mySocket::testSysCall(this->sendRes);
}

void    mySocket::recvReq(int sockfd, void *buf, int len, int flags)
{
    this->recvRes = recv(sockfd, buf, len, flags);
    mySocket::testSysCall(this->recvRes);
}