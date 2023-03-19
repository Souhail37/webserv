/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samajat <samajat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:49:48 by samajat           #+#    #+#             */
/*   Updated: 2023/03/19 19:44:08 by samajat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORMANAGER_HPP
# define ERRORMANAGER_HPP

#include "utils.hpp"
#include "exception.hpp"

//All the functions in this class are static as We don't need to create an object of this class

class errorManager
{
    private:
    typedef   std::string    Method_t;
    typedef   std::string    protocol_t;
    
    static const std::string    _validMethods[3];
    static const std::string    _validProtocol;

    public:
    static bool  isRequestValid(const request_t& request);
    

    
    private:
    static  bool        isMethodValid(Method_t Method);
    static  void        isProtocolValid(protocol_t protocol);
    static  void        isURIValid(const std::string& URI,location_t server_location);
    static  bool        isRestOfRequestValid(request_t request);
};


#endif