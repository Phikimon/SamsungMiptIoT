/*
 * server_default_params.hpp
 *
 *  Created on: 10 июл. 2017 г.
 *      Author: krautcat
 */

#ifndef TEST_SERVER_SERVER_DEFAULT_PARAMS_HPP_
#define TEST_SERVER_SERVER_DEFAULT_PARAMS_HPP_

#include <helper/default_params.hpp>

class DefaultServerConParams : public DefaultConParams
{
    public:
        DefaultServerConParams() {
            proto = "tcp";
            ip = "106.109.130.18";
            port = 1883;
        }
}

class DefaultServerTopicParams : public DefaultTopicParams
{
    public:
        DefaultServerTopicParams() {
            deveui = "807B85902000019A";
            devices = {"opt3001", "adc"};
        }
}

#endif /* TEST_SERVER_SERVER_DEFAULT_PARAMS_HPP_ */
