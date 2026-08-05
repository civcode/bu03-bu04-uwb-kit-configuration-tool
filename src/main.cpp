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
    std::cout << "Sending command: " << command << std::endl;
    uart.writeText(command + "\r\n");

    std::string received;
    uart.readText(received, 256);

    // PrintAllChar(received);
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

        // std::cout << "Sending test commands...\n";
        // test_command(uart, "AT");
        // test_command(uart, "AT+GETVER");
        // test_command(uart, "AT+GETWORKMODE");
        // test_command(uart, "AT+GETCFG");
        // test_command(uart, "AT+GETSENSOR");
        // test_command(uart, "AT+TESTLED");
        // // test_command(*uart, "AT+TESTOLED");
        // test_command(uart, "AT+DISTANCE");
        // test_command(uart, "AT+GETDEV");

        // std::cout << "Sending test commands (readText)...\n";
        // test_command2(uart, "AT+GETVER");
        // test_command2(uart, "AT+GETCFG");
        // test_command2(uart, "AT+GETDEV");

        BU04Handler handler(uart);

        BU04Handler::EResult result;
        std::string response;

        // result = handler.GetAt(response);
        // if (result == BU04Handler::EResult::kSuccess) {
        //     std::cout << "BU04 AT Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to get BU04 AT response\n";
        // }
        // std::cout << std::endl;

    

        std::string version;
        result = handler.GetVer(response, version);
        if (result == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Version: " << version << std::endl;
        } else {
            std::cerr << "Failed to get BU04 version\n";
        }
        std::cout << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int workMode;
        result = handler.GetWorkMode(response, workMode);
        if (result == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Work Mode: " << workMode << std::endl;
        } else {
            std::cerr << "Failed to get BU04 work mode\n";
        }
        std::cout << std::endl;

        std::string devCfg;
        BU04Handler::DeviceConfiguration deviceConfig;
        // while (!stop_requested.test()) {
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
        std::cout << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        // result = handler.Restart(response);
        // if (result == BU04Handler::EResult::kSuccess) {
        //     std::cout << "BU04 Restart Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to restart BU04\n";
        // }
        // std::cout << std::endl;
        
        // result = handler.Restore(response);
        // if (result == BU04Handler::EResult::kSuccess) {
        //     std::cout << "BU04 Restore Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to restore BU04\n";
        // }
        // std::cout << std::endl;
        // return 0;

        
        std::string devInfo;
        BU04Handler::TwrDeviceSetup setup;
        // while (!stop_requested.test()) {
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
        std::cout << std::endl;
        // }

        result = handler.GetDeca(response);
        if (result == BU04Handler::EResult::kSuccess) {
            // std::cout << "BU04 Deca Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 deca response\n";
        }
        std::cout << std::endl;

        result = handler.GetDList(response);
        if (result == BU04Handler::EResult::kSuccess) {
            // std::cout << "BU04 DList Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 dlist response\n";
        }
        std::cout << std::endl;

        result = handler.GetKList(response);
        if (result == BU04Handler::EResult::kSuccess) {
            // std::cout << "BU04 KList Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 klist response\n";
        }
        std::cout << std::endl;

        return 0;

        // deviceConfig.id = 1;
        // deviceConfig.role = 0;
        // deviceConfig.channel = 0;
        // deviceConfig.rate = 1;
        // const auto setCfgResult = handler.SetCfg(deviceConfig);
        // if (setCfgResult == BU04Handler::EResult::kSuccess) {
        //     std::cout << "Successfully set BU04 device configuration\n";
        // } else {
        //     std::cerr << "Failed to set BU04 device configuration\n";
        // }
        // std::cout << std::endl;


        // result = handler.Save(response);
        // if (result == BU04Handler::EResult::kSuccess) {
        //     std::cout << "BU04 Save Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to save BU04 configuration\n";
        // }

        float distance;
        std::string distanceInfo;
        // while (!stop_requested.test()) {
        const auto distanceResult = handler.GetDistance(distanceInfo, distance);
        if (distanceResult == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Distance Info: " << distanceInfo << std::endl;
            std::cout << "Distance: " << distance << " meters" << std::endl;
        } else {
            std::cerr << "Failed to get BU04 distance info\n";
        }
        std::cout << std::endl;
        // }

        result = handler.Restart(response);
        if (result == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Restart Response: " << response << std::endl;
        } else {
            std::cerr << "Failed to restart BU04\n";
        }
        std::cout << std::endl;

        result = handler.TestLed(response, 1);
        if (result == BU04Handler::EResult::kSuccess) {
            std::cout << "BU04 Test LED Response: " << response << std::endl;
        } else {
            std::cerr << "Failed to test BU04 LED\n";
        }
        std::cout << std::endl;

        return 0;

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}