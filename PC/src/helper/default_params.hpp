/*
 * default_params.hpp
 *
 *  Created on: 10 июл. 2017 г.
 *      Author: krautcat
 */

#ifndef SRC_HELPER_DEFAULT_PARAMS_HPP_
#define SRC_HELPER_DEFAULT_PARAMS_HPP_

#include <string>
#include <vector>

class DefaultConParams
{
    public:
        DefaultConParams() {
            proto = "tcp";
            ip = "10.55.192.151";
            port = 1883;
        }

        const std::string& getIp() const {
            return ip;
        }

        unsigned int getPort() const {
            return port;
        }

        const std::string& getProto() const {
            return proto;
        }

    protected:
        std::string proto;
        std::string ip;
        unsigned int port;
};

class DefaultTopicParams
{
    public:
        DefaultTopicParams() {
            deveui = "807B8590200001E0";
            devices = {"opt3001", "adc"};
        }

        const std::string& getDeveui() const {
            return deveui;
        }

        std::vector <std::string> getDevices() const {
            return devices;
        }

    protected:
        std::string deveui;
        std::vector <std::string> devices;
};

#endif /* SRC_HELPER_DEFAULT_PARAMS_HPP_ */
