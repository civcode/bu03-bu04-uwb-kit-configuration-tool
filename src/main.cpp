#include <atomic>
#include <array>
#include <chrono>
#include <csignal>
#include <string>
#include <iomanip>
#include <iostream>

#include <span>
#include <thread>

#include "uart/uart.hpp"
#include "uart/bu04_handler.hpp"

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
    uart.writeText(command + "\r\n");

    // const std::string received = uart.readSome();
    std::uint8_t buffer[256];
    const std::size_t bytesRead = uart.read(buffer, 256, std::chrono::milliseconds(500));
    const std::string received(
        reinterpret_cast<const char*>(buffer),
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

void test_command2(Uart& uart, const std::string& command)
{
    uart.writeText(command + "\r\n");

    std::string received;
    uart.readText(received, 256);

    std::cout << received << std::flush;
    std::cout << std::endl;


    // const std::string received = uart.readSome();
//     std::uint8_t buffer[256];
//     const std::size_t bytesRead = uart.read(buffer, 256, std::chrono::milliseconds(500));
//     const std::string received(
//         reinterpret_cast<const char*>(buffer),
//         bytesRead);

//     if (!received.empty()) {
//         std::cout << "Received " << received.size()
//                   << " bytes: " << std::endl << std::flush;
//         std::cout << received << std::flush;
//     } else {
//         std::cout << "Read timeout\n";
//     }
//     for (unsigned char ch : received) {
//         std::cout << std::hex
//                 << std::setw(2)
//                 << std::setfill('0')
//                 << static_cast<int>(ch)
//                 << ' ';
//     }

//     std::cout << std::dec << "\n\n";
}

int main(int argc, char* argv[])
{
    const std::string device =
        argc > 1 ? argv[1] : "/dev/ttyUSB0";

    std::signal(SIGINT, handle_signal);

    try {
        auto uart = Uart(device, B115200);

        std::cout << "Opened " << device << " at 115200 baud\n";

        std::cout << "Sending test commands...\n";
        // test_command(uart, "AT");
        // test_command(uart, "AT+GETVER");
        // test_command(uart, "AT+GETWORKMODE");
        // test_command(uart, "AT+GETCFG");
        // test_command(uart, "AT+GETSENSOR");
        // test_command(uart, "AT+TESTLED");
        // // test_command(*uart, "AT+TESTOLED");
        // test_command(uart, "AT+DISTANCE");
        // test_command(uart, "AT+GETDEV");

        std::cout << "Sending test commands (readText)...\n";
        test_command2(uart, "AT+GETVER");
        test_command2(uart, "AT+GETCFG");
        test_command2(uart, "AT+GETDEV");

        BU04Handler handler(uart);
        std::string version;
        const auto result = handler.GetVersion(version);
        if (result == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Version: " << version << std::endl;
        } else {
            std::cerr << "Failed to get BU04 version\n";
        }

        std::string devCfg;
        BU04Handler::DeviceConfiguration deviceConfig;
        const auto cfgResult = handler.GetCfg(devCfg, deviceConfig);
        if (cfgResult == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Device Configuration: " << devCfg << std::endl;
            std::cout << "ID: " << deviceConfig.id << std::endl;
            std::cout << "Role: " << deviceConfig.role << std::endl;
            std::cout << "Channel: " << deviceConfig.channel << std::endl;
            std::cout << "Rate: " << deviceConfig.rate << std::endl;
        } else {
            std::cerr << "Failed to get BU04 device configuration\n";
        }

        std::string devInfo;
        BU04Handler::TwrDeviceSetup setup;
        const auto devResult = handler.GetDev(devInfo, setup);
        if (devResult == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Device Info: " << devInfo << std::endl;
            std::cout << "Tag Capacity: " << setup.tagCapacity << std::endl;
            std::cout << "Antenna Delay: " << setup.antennaDelay << std::endl;
            std::cout << "Kalman Filter Enabled: " << std::boolalpha << setup.isKalmanFilterEnabled << std::endl;
            std::cout << "Kalman Q: " << setup.kalmanQ << std::endl;
            std::cout << "Kalman R: " << setup.kalmanR << std::endl;
            std::cout << "Correction Parameter A: " << setup.correctionParameterA << std::endl;
            std::cout << "Correction Parameter B: " << setup.correctionParameterB << std::endl;
            std::cout << "Positioning Enabled: " << std::boolalpha << setup.isPositioningEnabled << std::endl;
            std::cout << "Positioning Dimension: " << setup.positioningDimension << std::endl;
        } else {
            std::cerr << "Failed to get BU04 device info\n";
        }

        deviceConfig.id = 0;
        deviceConfig.role = 0;
        deviceConfig.channel = 0;
        deviceConfig.rate = 1;
        const auto setCfgResult = handler.SetCfg(deviceConfig);
        if (setCfgResult == BU04Handler::EResult::kSuccess) {
            std::cout << "Successfully set BU04 device configuration\n";
        } else {
            std::cerr << "Failed to set BU04 device configuration\n";
        }


        std::this_thread::sleep_for(std::chrono::seconds(1));



        // uart.writeAll("Hello from Linux C++!\r\n");
        // uart.writeAll("AT\r\n");

        // const std::string received = uart.readSome();
        // if (!received.empty()) {
        //     std::cout << "Received " << received.size()
        //               << " bytes: " << received << std::flush;
        // }

        // while (!stop_requested.test()) {
        //     const std::string received = uart.readSome();

        //     if (!received.empty()) {
        //         std::cout << "Received " << received.size()
        //                   << " bytes: " << received << std::flush;
        //     } else {
        //         std::cout << "Read timeout\n";
        //     }
        // }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}