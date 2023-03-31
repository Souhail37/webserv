/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samajat <samajat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:49:34 by samajat           #+#    #+#             */
/*   Updated: 2023/03/31 14:12:24 by samajat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errorManager.hpp"
#include <string>

simpleConfPars parser;

const std::string errorManager::_validMethods[3] = {"GET", "POST", "DELETE"};
const std::string errorManager::_validProtocol = "HTTP/1.1";
const std::string errorManager::_notAllowedMethods[5] = {"OPTIONS","HEAD","PUT","TRACE","CONNECT"};


void     errorManager::isMethodValid(Method_t Method, bool requestHasBody)
{
    if ((!requestHasBody && Method == "POST") || 
           (requestHasBody && (Method == "GET" || Method == "DELETE")))//Check if method is valid for the request body
        throw StatusCode(BAD_REQUEST);

    for (size_t i = 0; i < _validMethods->size(); i++)
        if (Method == _validMethods[i])//valid method must be in the list of valid methods in the config file
            return ;
    
    for (size_t i = 0; i < _notAllowedMethods->size(); i++)
        if (Method == _notAllowedMethods[i])
            throw StatusCode(METHOD_NOT_ALLOWED) ;
    throw StatusCode(BAD_REQUEST);
}



void     errorManager::isProtocolValid(protocol_t protocol)
{
    if (protocol == _validProtocol)
        return ;
    if (protocol.substr( 0, 5) == "HTTP/" && (protocol[5] != '0' && protocol[5] != '\0') )
        throw StatusCode(HTTP_VERSION_NOT_SUPPORTED);
    throw StatusCode(BAD_REQUEST);
}


bool     errorManager::isLocationRedirected(const std::string& URI,location_t server_location)
{
    location_t::iterator it = server_location.find(URI);
    if (it != server_location.end())
    {
        directive_t::iterator it_red = it->second.find("return");
        if (it_red != it->second.end())
        {
            std::string redirection = it_red->second;
                if (redirection[0] == '/')
                    redirection = redirection.substr(1);
            throw StatusCode();
        }
        return false;
    }
    return false;
}

int errorManager::isURIValid(const std::string& URI,location_t server_location) {
    
    if (URI[0] != '/')
        throw StatusCode(BAD_REQUEST);
    if (URI.size() > MAX_URI_SIZE)
        throw StatusCode(URI_TOO_LONG);
    location_t::iterator it = server_location.find(URI);

    if (it != server_location.end())
        return URI.size();
    size_t pos = URI.find_last_of('/');
    if (!pos )
        return 1;
    else if (pos != std::string::npos)
        return isURIValid(URI.substr(0, pos), server_location);
    throw StatusCode(NOT_FOUND);
    return -1;
}

//This function is performed when we check if the URI is valid and we need to define the final 
//directory or a file from where the file should be searched

void     defineFinalUri (header_t& header, int targetPathSize, location_t server_location)
{
    struct stat sb;
    
    //check redirections
    
    std::string root =  server_location[header.at("URI").substr(0, targetPathSize)]["root"];
    if (targetPathSize == 1 && header.at("URI").size() > 1)
        root += "/";
    header.at("URI") = root + header.at("URI").substr(targetPathSize);
    if (stat(header.at("URI").c_str(), &sb) == -1)
        throw StatusCode(NOT_FOUND);
}

bool     errorManager::isRequestValid(http_message_t &request)
{
    location_t     server_location = parser.get_server_locations(0);
    header_t              &header         = request.first;

    isMethodValid(header.find("Method")->second, !request.second.empty());
    isProtocolValid(header.find("Protocol")->second);
    
    int   targetPathSize = isURIValid(header.find("URI")->second, server_location);
    
    // std::pair <bool, directive_t::iterator> it2 = parser.get_directive(0, header["URI"], "max_body_size");
    // std::cout << "header[\"URI\"] "  << it2.first <<  header["URI"] << (static_cast<int>(request.second.size()) > atoi((it2.second->second).c_str())) << std::endl;
    // if (it2.first && static_cast<int>(request.second.size()) > atoi((it2.second->second).c_str()))
    //     throw StatusCode(REQUEST_ENTITY_TOO_LARGE);
    
    defineFinalUri(header, targetPathSize, server_location);

    //put it in its own function
    header_t::const_iterator it = header.find("host");
    if (it ==  header.end() || it->second.empty())
        throw StatusCode(BAD_REQUEST);
    for (std::string::const_iterator iter = it->second.begin(); iter != it->second.end(); iter++)
        if (isspace(*iter))
            throw StatusCode(BAD_REQUEST);

    return true;
}