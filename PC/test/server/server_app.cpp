/*
 * server_app.cpp
 *
 *  Created on: 10 июл. 2017 г.
 *      Author: krautcat
 */

#include <cstdlib>

#include <string>
#include <atomic>
#include <chrono>

#include <mqtt/async_client.h>

#include <helper/arg_parser.hpp>
#include <server/helper/con_params.hpp>
#include <server/helper/topic_params.hpp>

const std::string DFLT_SERVER_ADDRESS { "tcp://localhost:1883" };
const std::string DFLT_CLIENT_ID { "async_publish" };

const std::string TOPIC { "hello" };

const char* PAYLOAD1 = "Hello World!";
const char* PAYLOAD2 = "Hi there!";
const char* PAYLOAD3 = "Is anyone listening?";
const char* PAYLOAD4 = "Someone is always listening.";

const char* LWT_PAYLOAD = "Last will and testament.";

const int QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);

//
// A callback class for use with the main MQTT client.
//
class callback : public virtual mqtt::callback
{
    public:
        void connection_lost(const std::string& cause) override {
            std::cout << "\nConnection lost" << std::endl;
            if (!cause.empty())
                std::cout << "\tcause: " << cause << std::endl;
        }

        void delivery_complete(mqtt::delivery_token_ptr tok) override {
            std::cout << "\tDelivery complete for token: "
                    << (tok ? tok->get_message_id() : -1) << std::endl;
        }
};

//
// A base action listener.
//
class action_listener : public virtual mqtt::iaction_listener
{
    protected:
        void on_failure(const mqtt::token& tok) override {
            std::cout << "\tListener failure for token: "
                    << tok.get_message_id() << std::endl;
        }

        void on_success(const mqtt::token& tok) override {
            std::cout << "\tListener success for token: "
                    << tok.get_message_id() << std::endl;
        }
};

//
// A derived action listener for publish events.
//
class delivery_action_listener : public action_listener
{
        std::atomic <bool> done_;

        void on_failure(const mqtt::token& tok) override {
            action_listener::on_failure(tok);
            done_ = true;
        }

        void on_success(const mqtt::token& tok) override {
            action_listener::on_success(tok);
            done_ = true;
        }

    public:
        delivery_action_listener() :
                done_(false) {
        }
        bool is_done() const {
            return done_;
        }
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, const char* argv[]) {
    ConParams *mqtt_conn_params = nullptr;
    TopicParams *mqtt_topic_params = nullptr;

    ArgumentParser parser;
    parser.addArgument("--proto", 1);
    parser.addArgument("--ip", 1);
    parser.addArgument("--port", 1);
    parser.addArgument("--uuid", 1);
    parser.addArgument("--type", "+");
    parser.parse(argc, argv);

    std::string proto = "", ip = "";
    unsigned int port = 0;
    try {
//        proto = parser.retrieve <std::string> ("proto");
        ip = parser.retrieve <std::string> ("ip");
//        port = parser.retrieve <unsigned int> ("port");
    } catch (const std::out_of_range &e) {
        if (proto.empty() && ip.empty() && !port) {
            mqtt_conn_params = new ConParams();
        } else if (!ip.empty() && !port) {
            mqtt_conn_params = new ConParams(ip);
        } else if (!ip.empty() && port) {
            mqtt_conn_params = new ConParams(ip, port);
        }
    }

    std::string uuid = "";
    std::vector <std::string> type = {};

    std::cout << "Initializing for server '" << address << "'..." << std::endl;
    mqtt::async_client client(address, clientID);

    callback cb;
    client.set_callback(cb);

    mqtt::connect_options conopts;
    mqtt::message willmsg(TOPIC, LWT_PAYLOAD, 1, true);
    mqtt::will_options will(willmsg);
    conopts.set_will(will);

    std::cout << "  ...OK" << std::endl;

    try {
        std::cout << "\nConnecting..." << std::endl;
        mqtt::token_ptr conntok = client.connect(conopts);
        std::cout << "Waiting for the connection..." << std::endl;
        conntok->wait();
        std::cout << "  ...OK" << std::endl;

        // First use a message pointer.

        std::cout << "\nSending message..." << std::endl;
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, PAYLOAD1);
        pubmsg->set_qos(QOS);
        client.publish(pubmsg)->wait_for(TIMEOUT);
        std::cout << "  ...OK" << std::endl;

        // Now try with itemized publish.

        std::cout << "\nSending next message..." << std::endl;
        mqtt::delivery_token_ptr pubtok;
        pubtok = client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2), QOS, false);
        std::cout << "  ...with token: " << pubtok->get_message_id() << std::endl;
        std::cout << "  ...for message with "
                << pubtok->get_message()->get_payload().size() << " bytes"
                << std::endl;
        pubtok->wait_for(TIMEOUT);
        std::cout << "  ...OK" << std::endl;

        // Now try with a listener

        std::cout << "\nSending next message..." << std::endl;
        action_listener listener;
        pubmsg = mqtt::make_message(TOPIC, PAYLOAD3);
        pubtok = client.publish(pubmsg, nullptr, listener);
        pubtok->wait();
        std::cout << "  ...OK" << std::endl;

        // Finally try with a listener, but no token

        std::cout << "\nSending final message..." << std::endl;
        delivery_action_listener deliveryListener;
        pubmsg = mqtt::make_message(TOPIC, PAYLOAD4);
        client.publish(pubmsg, nullptr, deliveryListener);

        while (!deliveryListener.is_done()) {
            this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "OK" << std::endl;

        // Double check that there are no pending tokens

        auto toks = client.get_pending_delivery_tokens();
        if (!toks.empty())
            std::cout << "Error: There are pending delivery tokens!" << std::endl;

        // Disconnect
        std::cout << "\nDisconnecting..." << std::endl;
        conntok = client.disconnect();
        conntok->wait();
        std::cout << "  ...OK" << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
