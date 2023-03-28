/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fech-cha <fech-cha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 22:43:02 by fech-cha          #+#    #+#             */
/*   Updated: 2023/03/28 07:41:13 by fech-cha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>  
#include <string.h>
#include <unistd.h>

# define HTTP_PORT "80"
# define BACKLOG 100

//temp buff size
# define BUFFER_SIZE 1024
/* getaddrinfo => socket => bind => listen => accept*/
class mySocket
{
    private:
        struct addrinfo hints; //get info about the host network
        struct addrinfo *servinfo; //will point to the result 
        struct sockaddr_storage incomingStruct; //get info about incoming connections
        socklen_t incomingAddSize;
        
        int sockfd;
        int bindRes;
        int acceptSockFd;
        int sendRes;
        int recvRes;
        
    public:
        mySocket();
        ~mySocket();
        void    testSysCall(int fd);
        void    rerunServ(void);
        void    listenRequest(void);
        void    acceptConnection(void);
        void    closeConnection(void);
        void    sendReq(int sockfd, const void *buf, int len, int flags);
        void    recvReq(int sockfd, void *buf, int len, int flags);

        //getter functions
        int                 getSockFd(void) const;
        int                 getBindValue(void) const;
        int                 getAcceptFd(void) const;
};

#endif