#include <atomic>
#include <array>
#include <chrono>
#include <csignal>
#include <string>
#include <iomanip>
#include <iostream>

#include <span>
#include <thread>

#include "transport/uart.hpp"
#include "protocol/device_handler.hpp"
#include "configuration/device_configuration.hpp"
#include "configuration/configuration_file.hpp"
#include "configuration/serialization.hpp"

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

        DeviceHandler handler(uart);

        DeviceHandler::EResult result;
        std::string response;

        // result = handler.GetAt(response);
        // if (result == BU04Handler::EResult::kSuccess) {
        //     std::cout << "BU04 AT Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to get BU04 AT response\n";
        // }
        // std::cout << std::endl;

    

        std::string getVer;
        result = handler.GetVer(response, getVer);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Version: " << getVer << std::endl;
        } else {
            std::cerr << "Failed to get BU04 getVer\n";
        }
        std::cout << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int workMode;
        result = handler.GetWorkMode(response, workMode);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Work Mode: " << workMode << std::endl;
        } else {
            std::cerr << "Failed to get BU04 work mode\n";
        }
        std::cout << std::endl;

        std::string devCfg;
        DeviceParameters getCfg;
        // while (!stop_requested.test()) {
        const auto cfgResult = handler.GetCfg(devCfg, getCfg);
        if (cfgResult == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Device Configuration: " << devCfg << std::endl;
            std::cout << "ID: " << getCfg.id << std::endl;
            std::cout << "Role: " << getCfg.role << std::endl;
            std::cout << "Channel: " << getCfg.channel << std::endl;
            std::cout << "Rate: " << getCfg.rate << std::endl;
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
        GetDevParam getDev;
        // while (!stop_requested.test()) {
        result = handler.GetDev(devInfo, getDev);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Device Info: " << devInfo << std::endl;
            std::cout << "Tag Capacity: " << getDev.tagCapacity << std::endl;
            std::cout << "Antenna Delay: " << getDev.antennaDelay << std::endl;
            std::cout << "Kalman Filter Enabled: " << std::boolalpha << getDev.isKalmanFilterEnabled << std::endl;
            std::cout << "Kalman Q: " << getDev.kalmanQ << std::endl;
            std::cout << "Kalman R: " << getDev.kalmanR << std::endl;
            std::cout << "Correction Parameter A: " << getDev.correctionParameterA << std::endl;
            std::cout << "Correction Parameter B: " << getDev.correctionParameterB << std::endl;
            std::cout << "Positioning Enabled: " << std::boolalpha << getDev.isPositioningEnabled << std::endl;
            std::cout << "Positioning Dimension: " << getDev.positioningDimension << std::endl;
        } else {
            std::cerr << "Failed to get BU04 device info\n";
        }
        std::cout << std::endl;
        // }

        getDev.tagCapacity = 2;
        getDev.correctionParameterA = 1.0f;
        getDev.correctionParameterB = 0.0f;
        result = handler.SetDev(getDev);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "Successfully set BU04 device info\n";
        } else {
            std::cerr << "Failed to set BU04 device info\n";
        }
        std::cout << std::endl;

        result = handler.GetDeca(response);
        if (result == DeviceHandler::EResult::kSuccess) {
            // std::cout << "BU04 Deca Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 deca response\n";
        }
        std::cout << std::endl;

        result = handler.GetDList(response);
        if (result == DeviceHandler::EResult::kSuccess) {
            // std::cout << "BU04 DList Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 dlist response\n";
        }
        std::cout << std::endl;

        result = handler.GetKList(response);
        if (result == DeviceHandler::EResult::kSuccess) {
            // std::cout << "BU04 KList Response: " << response << std::endl;
            handler.PrintAllChar(response);
        } else {
            std::cerr << "Failed to get BU04 klist response\n";
        }
        std::cout << std::endl;

        int uwbMode;
        result = handler.GetUwbMode(response, uwbMode);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 UWB Mode: " << uwbMode << std::endl;
        } else {
            std::cerr << "Failed to get BU04 UWB mode\n";
        }
        std::cout << std::endl;

        uwbMode = 0;
        result = handler.SetUwbMode(uwbMode);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "Successfully set BU04 UWB mode to " << uwbMode << "\n";
        } else {
            std::cerr << "Failed to set BU04 UWB mode\n";
        }
        std::cout << std::endl;

        int pdoaOffset = 0;
        result = handler.SetPdoaOffset(pdoaOffset);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "Successfully set BU04 PDoA offset to " << pdoaOffset << " degrees\n";
        } else {
            std::cerr << "Failed to set BU04 PDoA offset\n";
        }
        std::cout << std::endl;

        int rngOffset = 0;
        result = handler.SetRngOffset(rngOffset);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "Successfully set BU04 range offset to " << rngOffset << " mm\n";
        } else {
            std::cerr << "Failed to set BU04 range offset\n";
        }
        std::cout << std::endl; 

        PdoaGetCfgParam pdoaCfg;
        result = handler.GetPdoaCfg(response, pdoaCfg);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 PDoA Configuration: " << response << std::endl;
            std::cout << "Dlist: " << pdoaCfg.dlist << std::endl;
            std::cout << "KList: " << pdoaCfg.klist << std::endl;
            std::cout << "Net: " << pdoaCfg.net << std::endl;
            std::cout << "AncID: " << pdoaCfg.anchId << std::endl;
            std::cout << "Rate: " << pdoaCfg.rate << std::endl;
            std::cout << "Filter Enabled: " << std::boolalpha << pdoaCfg.isFilterEnabled << std::endl;
            std::cout << "UserCmd: " << pdoaCfg.userCmd << std::endl;
            std::cout << "PDoA Offset: " << pdoaCfg.pdoaOffset << std::endl;
            std::cout << "Range Offset: " << pdoaCfg.rngOffset << std::endl;
        } else {
            std::cerr << "Failed to get BU04 PDoA configuration\n";
        }
        std::cout << std::endl;

        DeviceConfiguration deviceConfig{
            .getCfg = getCfg,
            .twrSetup = getDev,
            .pdoaConfig = pdoaCfg
        };

        ConfigurationFile::Save("device_config.json", deviceConfig);
        // ConfigurationFile::Save("device_config.json", deviceParam);


        // DeviceHandler::TagParameters tagParams{
        //     .a64 = "000000004E818834",
        //     .a16 = 8834,
        //     .F = 1,
        //     .S = 64,
        //     .M = 0
        // };
        // result = handler.AddTag(response, tagParams);
        // if (result == DeviceHandler::EResult::kSuccess) {
        //     std::cout << "Successfully added BU04 tag\n";
        // } else {
        //     std::cerr << "Failed to add BU04 tag\n";
        // }
        // std::cout << std::endl;

        return 0;

        // getCfg.id = 1;
        // getCfg.role = 0;
        // getCfg.channel = 0;
        // getCfg.rate = 1;
        // const auto setCfgResult = handler.SetCfg(deviceParam);
        // if (setCfgResult == DeviceHandler::EResult::kSuccess) {
        //     std::cout << "Successfully set BU04 device configuration\n";
        // } else {
        //     std::cerr << "Failed to set BU04 device configuration\n";
        // }
        // std::cout << std::endl;


        // result = handler.Save(response);
        // if (result == DeviceHandler::EResult::kSuccess) {
        //     std::cout << "BU04 Save Response: " << response << std::endl;
        // } else {
        //     std::cerr << "Failed to save BU04 configuration\n";
        // }

        float distance;
        std::string distanceInfo;
        // while (!stop_requested.test()) {
        const auto distanceResult = handler.GetDistance(distanceInfo, distance);
        if (distanceResult == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Distance Info: " << distanceInfo << std::endl;
            std::cout << "Distance: " << distance << " meters" << std::endl;
        } else {
            std::cerr << "Failed to get BU04 distance info\n";
        }
        std::cout << std::endl;
        // }

        result = handler.Restart(response);
        if (result == DeviceHandler::EResult::kSuccess) {
            std::cout << "BU04 Restart Response: " << response << std::endl;
        } else {
            std::cerr << "Failed to restart BU04\n";
        }
        std::cout << std::endl;

        result = handler.TestLed(response, 1);
        if (result == DeviceHandler::EResult::kSuccess) {
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