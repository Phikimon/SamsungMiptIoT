#include <cstdlib>
#include <cstring>
#include <cctype>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <regex>

#include <fstream>

#include "mqtt/async_client.h"

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

//#include <SFML/Window/Window.hpp>

#include "helper/arg_parser.hpp"
#include "helper/json_helpers.hpp"
#include "helper/params.hpp"
#include "helper/default_params.hpp"

#include "pump_control.cxx"

const std::string CLIENT_ID { "sync_consume_cpp" };
const std::string TOPIC { "devices/lora/#" };

const int QOS = 1;
const char NUM_RETRY_ATTEMPTS = 5;

// Callbacks for the success or failures of requested actions.

class ActionListener : public virtual mqtt::iaction_listener
{
    public:
        ActionListener(const std::string &name) :
                _name(name) {
        }

    private:
        std::string _name;

        void on_failure(const mqtt::token &tok) override {
            std::cout << _name << " failure";
            if (tok.get_message_id() != 0)
                std::cout << " for token: [" << tok.get_message_id() << "]"
                        << std::endl << std::endl;
        }

        void on_success(const mqtt::token &tok) override {
            std::cout << _name << " success";
            if (tok.get_message_id() != 0)
                std::cout << " for token: [" << tok.get_message_id() << "]"
                        << std::endl;
            auto top = tok.get_topics();
            if (top && !top->empty())
                std::cout << "\ttoken topic: '" << (*top)[0] << "', ...";
            std::cout << std::endl << std::endl;
        }
};

class Callback : public virtual mqtt::callback,
        public virtual mqtt::iaction_listener
{
    public:
        Callback(mqtt::async_client &cli_cl, mqtt::connect_options &connOpts) :
                nretry(0), cli(cli_cl), connOpts(connOpts),
                subListener("Subscription") {
					
        }

    private:
        // Counter for the number of connection retries
        int nretry;
        // Options to use if we need to reconnect
        mqtt::async_client &cli;
        // An action listener to display the result of actions.
        mqtt::connect_options connOpts;
        ActionListener subListener;
	
		// Данные от датчиков + данные от конфигурационного файла
		Pump pump_data;

        // This deomonstrates manually reconnecting to the broker by calling
        // connect() again. This is a possibility for an application that keeps
        // a copy of it's original connect_options, or if the app wants to
        // reconnect with different options.
        // Another way this can be done manually, if using the same options, is
        // to just call the async_client::reconnect() method.
        void reconnect() {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
            try {
                cli.connect(connOpts, nullptr, *this);
            } catch (const mqtt::exception &exc) {
                std::cerr << "Error: " << exc.what() << std::endl;
                exit(1);
            }
        }

        // Re-connection failure
        void on_failure(const mqtt::token& tok) override {
            std::cout << "Connection failed" << std::endl;
            if (++nretry > NUM_RETRY_ATTEMPTS)
                exit(1);
            reconnect();
        }

        // Re-connection success
        void on_success(const mqtt::token& tok) override {
            std::cout << "Connection success" << std::endl;
            std::cout << "Subscribing to topic '" << TOPIC << "'\n"
                    << "\tfor client " << CLIENT_ID << " using QoS" << QOS
                    << "\n" << "\nPress Q<Enter> to quit\n" << std::endl;

            cli.subscribe(TOPIC, QOS, nullptr, subListener);
        }

        // Callback for when the connection is lost.
        // This will initiate the attempt to manually reconnect.
        void connection_lost(const std::string& cause) override {
            std::cout << "\nConnection lost" << std::endl;
            if (!cause.empty())
                std::cout << "\tcause: " << cause << std::endl;

            std::cout << "Reconnecting..." << std::endl;
            nretry = 0;
            reconnect();
        }

        // Callback for when a message arrives.
        void message_arrived(mqtt::const_message_ptr msg) override {
			
			Json::Value root_config;
			Json::Value root_devices;
			
			std::ifstream config_file("config.json", std::ifstream::binary); 	//Адресс уточнить
			config_file >> root_config;
			if(root_config["watering"]["mode"].asString() == "manual") {
				pump.mode = MANUAL;
				if(root_config["watering"]["mode"]["manual"].asString() == "on")
					pump.manual_state = ON;
				else
					pump.manual_state = OFF;
			}
			else
				pump.mode = AUTO;
			pump.frequency = root_config["watering"]["frequency"].asInt();
			pump.hum_max = root_config["watering"]["max_ok"].asInt();
			pump.hum_min = root_config["watering"]["min_ok"].asInt();
			
			std::ifstream dev_file("dev.json", std::ifstream::binary);	 //Адресс уточнить
			config_file >> root_devices;
			
			if (root_devices["map"]["controller"]["name"]=="pump") {
				pump.ID = root_devices["map"]["controller"]["ID"].asString();
			}
			
			
            std::cout << "Message arrived" << std::endl;
            std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
            std::cout << "\tpayload: '" << msg->to_string() << "'\n"
                    << std::endl;

            std::string topic = msg->get_topic();
            std::vector <std::string> topic_strings = jsonHelper::split(topic,
                    '/');
            std::string lora_deveui = topic_strings[2];
            std::string device = topic_strings[3];

           
            Json::Reader json_reader;

            if (device == "adc") {
                if (json_reader.parse(msg->to_string(), root)) {
                                        
                    pump_data.humidity = root["data"]["adc1"].asFloat();
                    
                }
            }
        }

        void delivery_complete(mqtt::delivery_token_ptr token) override {
        }
};

int main(int argc, const char* argv[]) {
    ConParams<DefaultConParams> *mqtt_conn_params = nullptr;
    TopicParams<DefaultTopicParams> *mqtt_topic_params = nullptr;

    ArgumentParser parser;
    parser.addArgument("--proto", 1);
    parser.addArgument("--ip", 1);
    parser.addArgument("--addr", 1);
    parser.addArgument("--port", 1);
    parser.addArgument("--uuid", 1);
    parser.addArgument("--type", "+");
    parser.parse(argc, argv);

    std::string proto = "", ip = "", addr = "", uuid = "";
    unsigned int port = 0;
    std::vector <std::string> type = {};
    try {
        proto = parser.retrieve <std::string> ("proto");
        ip = parser.retrieve <std::string> ("ip");
        addr = parser.retrieve <std::string> ("addr");
//        port = parser.retrieve <unsigned int> ("port");
    } catch (const std::out_of_range &e) {
        if (proto.empty() && ip.empty() && !port) {
            mqtt_conn_params = new ConParams<DefaultConParams>();
        } else if (!ip.empty() && !port) {
            mqtt_conn_params = new ConParams<DefaultConParams>(ip);
        } else if (!ip.empty() && port) {
            mqtt_conn_params = new ConParams<DefaultConParams>(ip, port);
        }
    }
    if (mqtt_conn_params == nullptr) {
        mqtt_conn_params = new ConParams<DefaultConParams>(ip);
    }

    try {
        uuid = parser.retrieve <std::string> ("uuid");
        //type = parser.retrieve <std::vector <std::string>> ("type");
    } catch (const std::out_of_range &e) {
        if (uuid.empty() && type.empty()) {
            mqtt_topic_params = new TopicParams<DefaultTopicParams>();
        } else if (!uuid.empty() && type.empty()) {
            mqtt_topic_params = new TopicParams<DefaultTopicParams>(uuid);
        }
    }
    if (mqtt_topic_params == nullptr) {
        mqtt_topic_params = new TopicParams<DefaultTopicParams>(uuid, type);
    }

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    std::cout << mqtt_conn_params->getServerAddr();

    mqtt::async_client client(mqtt_conn_params->getServerAddr(), CLIENT_ID);

    Callback cb(client, connOpts);
    client.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        std::cout << "Connecting to the MQTT server..." << std::flush;
        client.connect(connOpts, nullptr, cb);
    } catch (const mqtt::exception&) {
        std::cerr << "\nERROR: Unable to connect to MQTT server: '"
                << mqtt_conn_params->getServerAddr() << "'" << std::endl;
        return 1;
    }

//    sf::Window window;
//    window.create(sf::VideoMode(800, 600), "My window");

    // Just block till user tells us to quit.

    while (std::tolower(std::cin.get()) != 'q')
        ;

    // Disconnect

    try {
        std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
        client.disconnect()->wait();
        std::cout << "OK" << std::endl;
    } catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    // Подчищаем динамически выделенную память
    delete mqtt_conn_params;
    delete mqtt_topic_params;

    return 0;
}
