/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responding.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samajat <samajat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 16:58:01 by samajat           #+#    #+#             */
/*   Updated: 2023/04/09 15:16:29 by samajat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "responding.hpp"
#include <string>


responsePreparation::responsePreparation(const http_message_t& request, const  StatusCode& statusCode):_request(request), _statusCode(statusCode), _response("")
{
    if(_statusCode.is_error_status())
        prepare_error_response();
    else if (_request.header["Method"] == "GET")
        exceute_get();
    // else if (_request.header["Method"] == "POST")
    //     exceute_post();
    // else if (_request.header["Method"] == "DELETE")
    //     exceute_delete();
}

std::vector<char> responsePreparation::get_response()
{
    return _final_response;
}

void responsePreparation::prepare_statusLine()
{
    _response += "HTTP/1.1 " + std::string(_statusCode.what());
    _response += CRLF;
}


void    responsePreparation::add_CRLF()
{
    _response += CRLF;
}


void responsePreparation::prepare_server_name()
{
    _response += "Server: Webserv/1.0";
    add_CRLF();
}

void responsePreparation::prepare_date()
{
    _response += "Date: " + utility::get_date();
    add_CRLF();
}

// void responsePreparation::prepare_location()
// {
//     if (_statusCode.get_redir_location() != "")
//     {
//         _response += "Location: " + _statusCode.get_redir_location();
//         add_CRLF();
//     }
// }

// void responsePreparation::prepare_content_length()
// {
//     size_t pos = _response.find(CRLF CRLF);
//     if (pos == std::string::npos)
//         return;
//     std::string body_file = _response.substr(pos + 4);
//     if (_response.substr(pos, 4) == CRLF CRLF)
//         std::cout << "|||||||||||||\n";        
//     // std::cout << ">>>>>>>> " << body_file.length() << std::endl;
//     if (body_file.empty() == false)
//         _response.insert(pos, std::string(CRLF) + "Content-Length: " + std::to_string(body_file.length() - 2));
// }

// void responsePreparation::prepare_allow()
// {
//     if (_statusCode.get_status_code() == METHOD_NOT_ALLOWED)
//     {
//         _response += "Allow: GET, POST, DELETE";
//         add_CRLF();
//     }
// }



// void responsePreparation::prepare_other_headers()
// {
//     _response += "Server: Webserv/1.0";
//     _response += CRLF;
//     _response += "Date: " + utility::get_date();
//     _response += CRLF;
//     if (_statusCode.get_redir_location() != "")
//     {
//         _response += "Location: " + _statusCode.get_redir_location();
//         _response += CRLF;
//         _response += CRLF;
//     }
// }



// void responsePreparation::prepare_body() //I'm gonna assume for now that the uri is a file
// {
//     std::string appropriate_page ;

//     if (_statusCode.get_redir_location() != "")
//         return;
//     if (_statusCode.is_error_status())
//     {
//         appropriate_page = _statusCode.get_associated_page();
//         add_CRLF();add_CRLF();
//         _response.append(appropriate_page);
//         return;
//     }
//     if (utility::check_file_or_directory(_request.header.at("URI")) == S_DIRECTORY && 
//                         parser.get_server_locations(0).find(_request.targeted_Location)->second.find("autoindex")->second == "on")
//                         {
//                             appropriate_page = utility::list_directory(_request.header["URI"]);
//                         }
//     else
//         appropriate_page = utility::get_file_content(_request.header["URI"]);
//     add_CRLF();add_CRLF();
//     _response.append(appropriate_page);
// }


// void responsePreparation::prepare_content_length()
// {
//     _response += "Content-Length: " + std::to_string(_response.length());
//     _response += CRLF;
// }

void    responsePreparation::exceute_get()
{
    prepare_statusLine();
    prepare_server_name();
    prepare_date();
    
    // prepare_location();
    // prepare_allow();
    _response += "Content-Length: " + std::to_string(utility::get_file_content(_request.header["URI"]).size());
    // std::cout << "length"<<  utility::get_file_content(_request.header["URI"]).length() << std::endl;
    // std::cout << "size  "<<utility::get_file_content(_request.header["URI"]).size() << std::endl;
    
    // _response+= CRLF;
    // _response+= "Content-Type: image/jpg";
    _response+= CRLF;
    _response+= CRLF;
    _final_response.insert(_final_response.end(), _response.begin(), _response.end());
    _final_response.insert(_final_response.end(), utility::get_file_content(_request.header["URI"]).begin(), utility::get_file_content(_request.header["URI"]).end());
    // _response+= utility::get_file_content(_request.header["URI"]);

    // prepare_body();
    // prepare_content_length();
}

// void    responsePreparation::exceute_post()
// {
//     prepare_statusLine();

// }

// void    responsePreparation::exceute_delete()
// {
// }


void   responsePreparation::prepare_error_response()
{
    prepare_statusLine();
    prepare_server_name();
    prepare_date();
    // prepare_body();
    // prepare_content_length();
}