#include <atomic>
#include <csignal>
#include <string>
#include <iostream>

#include <thread>

#include "transport/uart.hpp"
#include "protocol/device_handler.hpp"
#include "configuration/device_configuration.hpp"
#include "configuration/configuration_file.hpp"
#include "configuration/serialization.hpp"

#include "cxxopts.hpp"

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

int main(int argc, const char* argv[])
{
    const std::string device = "/dev/ttyUSB0";

    std::signal(SIGINT, handle_signal);

    cxxopts::Options options(argv[0], "Command-line interface for BU04 device");

    options.add_options()
        ("d,device", "Serial device path", cxxopts::value<std::string>()->default_value(device))
        ("e,export", "Export device configuration to JSON file", cxxopts::value<std::string>()
            ->implicit_value("device_config.json")
        )
        ("h,help", "Print help");
        

    auto result = options.parse(argc, argv);

    if (result.count("help") || result.arguments().empty()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::cout << "Using device: " << result["device"].as<std::string>() << std::endl;
    std::cout << "Exporting configuration to: " << result["export"].as<std::string>() << std::endl;

    return 0;
    /*
    TODO
    - get device configuration
    - match parameter to config command
    - get parameters
    - changer parameter
    - get parameters again
    - verify change    
    - save configuration
    */
    

    try {
        auto uart = Uart(device, B115200);

        DeviceHandler handler(uart);

        DeviceHandler::EResult errorCode;
        std::string response;

        if (result.count("export")) {
            DeviceConfiguration deviceConfig;
            errorCode = handler.GetDeviceConfiguration(deviceConfig);
            if (errorCode == DeviceHandler::EResult::kSuccess) {
                // std::cout << "Device configuration retrieved successfully." << std::endl;
                // std::string jsonString = serializeToJson(deviceConfig);
                // std::cout << "Device Configuration (JSON):" << std::endl;
                // std::cout << jsonString << std::endl;
                ConfigurationFile::Save(result["export"].as<std::string>(), deviceConfig);
            } else {
                std::cerr << "Failed to retrieve device configuration." << std::endl;
            }
        }
        
        
        // else if (result.count("import")) {
        //     // Load configuration from JSON file
        //     ConfigurationFile configFile("device_config.json");
        //     DeviceConfiguration deviceConfig = configFile.load();

        //     // Set device configuration
        //     errorCode = handler.SetCfg(deviceConfig.deviceParam);
        //     if (errorCode == DeviceHandler::EResult::kSuccess) {
        //         std::cout << "Device configuration set successfully." << std::endl;
        //     } else {
        //         std::cerr << "Failed to set device configuration." << std::endl;
        //     }
        // } else {
        //     // Default behavior: Get and print device configuration
        //     DeviceConfiguration deviceConfig;
        //     errorCode = handler.GetDeviceConfiguration(deviceConfig);
        //     if (errorCode == DeviceHandler::EResult::kSuccess) {
        //         std::cout << "Device configuration retrieved successfully." << std::endl;
        //         std::string jsonString = serializeToJson(deviceConfig);
        //         std::cout << "Device Configuration (JSON):" << std::endl;
        //         std::cout << jsonString << std::endl;
        //     } else {
        //         std::cerr << "Failed to retrieve device configuration." << std::endl;
        //     }
            

        // }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
