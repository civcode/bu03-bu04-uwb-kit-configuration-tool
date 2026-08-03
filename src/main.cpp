#include <atomic>
#include <chrono>
#include <csignal>
#include <string>
#include <iostream>
#include <thread>

#include "uart/uart.hpp"

namespace 
{
    std::atomic_flag stop_requested;

    extern "C" void handle_signal(int signal)
    {
        if (signal == SIGINT) {
            std::cout << "\nSIGINT received, stopping...\n";
            stop_requested.test_and_set();
        }
    }

}

void test_command(Uart& uart, const std::string& command)
{
    std::cout << "Sending command: " << command << std::endl << std::flush;
    uart.writeAll(command + "\r\n");

    // const std::string received = uart.readSome();
    const std::string received = uart.readAll();
    if (!received.empty()) {
        std::cout << "Received " << received.size()
                  << " bytes: " << std::endl << std::flush;
        std::cout << received << std::flush;
    } else {
        std::cout << "Read timeout\n";
    }
}

int main(int argc, char* argv[])
{
    const std::string device =
        argc > 1 ? argv[1] : "/dev/ttyUSB0";

    std::signal(SIGINT, handle_signal);

    try {
        Uart uart(device, B115200);

        std::cout << "Opened " << device << " at 115200 baud\n";

        std::cout << "Sending test commands...\n";
        test_command(uart, "AT");
        test_command(uart, "AT+GETVER");
        test_command(uart, "AT+GETWORKMODE");
        test_command(uart, "AT+GETCFG");
        test_command(uart, "AT+GETSENSOR");
        test_command(uart, "AT+TESTLED");
        // test_command(uart, "AT+TESTOLED");
        test_command(uart, "AT+DISTANCE");
        test_command(uart, "AT+GETDEV");

  
        std::this_thread::sleep_for(std::chrono::seconds(1));



        // uart.writeAll("Hello from Linux C++!\r\n");
        // uart.writeAll("AT\r\n");

        const std::string received = uart.readSome();
        if (!received.empty()) {
            std::cout << "Received " << received.size()
                      << " bytes: " << received << std::flush;
        }

        while (!stop_requested.test()) {
            const std::string received = uart.readSome();

            if (!received.empty()) {
                std::cout << "Received " << received.size()
                          << " bytes: " << received << std::flush;
            } else {
                std::cout << "Read timeout\n";
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}