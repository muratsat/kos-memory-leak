#include <iostream>
#include <string_view>

#include "general.h"
#include "subscriber.h"
#include "gpiomodule.h"

using namespace std::literals;

constexpr auto Topic = "test"sv;

Subscriber::Subscriber(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void Subscriber::on_connect(int rc)
{
    std::cout << app::AppTag << "ON COUT Subscriber connected with code "
              << rc << std::endl;

    std::cerr << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    if (rc == 0)
    {
        subscribe(NULL, Topic.data());
    }
}


int gpioInitflag = 0;

void Subscriber::on_message(const struct mosquitto_message *message)
{
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cerr << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload << std::endl;

        std::string cmd = std::string(static_cast<char*>(message->payload));
        gpiomain(cmd);
#if 0
        if(!gpioInitflag) {
           std::cerr << "Try init GPIO" << std::endl;

           gpioInitAll();
           gpioInitflag = 1;
        }
        std::cerr << "Try output on GPIO" << std::endl;

        trajectoryCmd();
#endif

    }
}

void Subscriber::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}
