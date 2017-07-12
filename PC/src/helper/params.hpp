/*
 * ConParams.h
 *
 *  Created on: 6 июл. 2017 г.
 *      Author: krautcat
 */

#ifndef SRC_HELPER_PARAMS_HPP_
#define SRC_HELPER_PARAMS_HPP_

#include <string>
#include <vector>
#include <tuple>

template<class Defaults>
class ConParams
{
    public:
        ConParams() {
            Defaults def;
            __ip = def.getIp();
            __port = def.getPort();
            __proto = def.getProto();
        }

        ConParams(std::string ip) :
                __ip(ip) {
            Defaults def;
            __port = def.getPort();
            __proto = def.getProto();
        }

        ConParams(std::string ip, unsigned int port) :
                __ip(ip), __port(port) {
            Defaults def;
            __proto = def.getProto();
        }

        virtual ~ConParams() {
        }

        void setProto(std::string proto) {
            __proto = proto;
        }

        void setIp(std::string ip) {
            __ip = ip;
        }
        ;
        void setPort(unsigned int port) {
            __port = port;
        }

        const std::string setAddr(std::string ip, unsigned int port) {
            setIp(ip);
            setPort(port);
            return getServerAddr();
        }

        const std::string getServerAddr() {
            return __proto + "://" + __ip + ":" + std::to_string(__port);
        }

    protected:
        std::string __proto;
        std::string __ip;
        unsigned int __port;
};

template<class Defaults>
class TopicParams
{
    public:
        TopicParams() {
            Defaults def;
            __deveui = def.getDeveui();
            __devices = def.getDevices();

        }

        TopicParams(std::string &deveui) :
                __deveui(deveui) {
            Defaults def;
            __devices = def.getDevices();
        }

        TopicParams(std::string &deveui, std::vector <std::string> &devices) :
                __deveui(deveui), __devices(devices) {
        }

        virtual ~TopicParams() {
        }

        void setDevEui(std::string &eui) {
            __deveui = eui;
        }

        void setDevices(std::string devices[], size_t num_devices) {
            for (size_t n = 0; n++; n < num_devices) {
                __devices.push_back(devices[n]);
            }
        }

        void setDevices(std::vector <std::string> &dev_vector) {
            __devices = dev_vector;
        }

        std::tuple <std::string, std::vector <std::string>> getParams() {
            return std::make_tuple(__deveui, __devices);
        }

    protected:
        std::string __deveui;
        std::vector <std::string> __devices;

};

//

#endif /* SRC_HELPER_PARAMS_HPP_ */
