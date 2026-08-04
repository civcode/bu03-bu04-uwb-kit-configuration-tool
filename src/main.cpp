#include <atomic>
#include <chrono>
#include <csignal>
#include <string>
#include <iomanip>
#include <iostream>

#include <span>
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
    std::string commandWithCRLF = command + "\r\n";
    std::span<const std::uint8_t> commandSpan(
        reinterpret_cast<const std::uint8_t*>(commandWithCRLF.data()),
        commandWithCRLF.size());
    uart.write(commandSpan);

    // const std::string received = uart.readSome();
    std::array<std::uint8_t, 256> bufferStorage{};
    std::span<std::uint8_t> buffer(bufferStorage);
    const std::size_t bytesRead = uart.read(buffer, std::chrono::milliseconds(500), std::chrono::milliseconds(50));
    const std::string received(
        reinterpret_cast<const char*>(buffer.data()),
        bytesRead);
    if (!received.empty()) {
        std::cout << "Received " << received.size()
                  << " bytes: " << std::endl << std::flush;
        std::cout << received << std::flush;
    } else {
        std::cout << "Read timeout\n";
    }
    for (unsigned char ch : received) {
        std::cout << std::hex
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(ch)
                << ' ';
    }

    std::cout << std::dec << "\n\n";
}

int main(int argc, char* argv[])
{
    const std::string device =
        argc > 1 ? argv[1] : "/dev/ttyUSB0";

    std::signal(SIGINT, handle_signal);

    try {
        auto uart = Uart::open(device, B115200);

        std::cout << "Opened " << device << " at 115200 baud\n";

        std::cout << "Sending test commands...\n";
        test_command(*uart, "AT");
        test_command(*uart, "AT+GETVER");
        test_command(*uart, "AT+GETWORKMODE");
        test_command(*uart, "AT+GETCFG");
        test_command(*uart, "AT+GETSENSOR");
        test_command(*uart, "AT+TESTLED");
        // test_command(*uart, "AT+TESTOLED");
        test_command(*uart, "AT+DISTANCE");
        test_command(*uart, "AT+GETDEV");

  
        std::this_thread::sleep_for(std::chrono::seconds(1));



        // uart.writeAll("Hello from Linux C++!\r\n");
        // uart.writeAll("AT\r\n");

        const std::string received = uart->readSome();
        if (!received.empty()) {
            std::cout << "Received " << received.size()
                      << " bytes: " << received << std::flush;
        }

        while (!stop_requested.test()) {
            const std::string received = uart->readSome();

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