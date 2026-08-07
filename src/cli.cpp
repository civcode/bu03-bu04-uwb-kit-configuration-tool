#include <atomic>
#include <csignal>
#include <string>
#include <iostream>

#include <thread>

#include "transport/uart.hpp"
#include "protocol/device_handler.hpp"
#include "protocol/device_configuration.hpp"
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
        ("device", "Serial device path", cxxopts::value<std::string>()->default_value(device))
        ("export", "Export device configuration to JSON file", cxxopts::value<std::string>()
            ->default_value("device_config.json")
            ->implicit_value("device_config.json"))
        ("uwb_mode", "UWB Mode", cxxopts::value<int>(), "(int)")
        ("dev_id", "Device ID", cxxopts::value<int>(), "(int)")
        ("dev_role", "Device Role", cxxopts::value<int>(), "(int)")
        ("dev_channel", "Device Channel", cxxopts::value<int>(), "(int)")
        ("dev_rate", "Device Rate", cxxopts::value<int>(), "(int)")
        ("twr_tag_cap", "TWR Tag Capacity", cxxopts::value<int>(), "(int)")
        ("twr_antenna_delay", "TWR Antenna Delay", cxxopts::value<int>(), "(int)")
        ("twr_kalman_filter_enable", "TWR Kalman Filter Enable", cxxopts::value<int>(), "(int)")
        ("twr_kalman_Q", "TWR Kalman Filter Q", cxxopts::value<double>(), "(double)")
        ("twr_kalman_R", "TWR Kalman Filter R", cxxopts::value<double>(), "(double)")
        ("twr_correction_a", "TWR Correction parameter A", cxxopts::value<double>(), "(double)")
        ("twr_correction_b", "TWR Correction parameter B", cxxopts::value<double>(), "(double)")
        ("twr_positioning_enable", "TWR Positioning Enable", cxxopts::value<int>(), "(int)")
        ("twr_positioning_dimension", "TWR Positioning Dimension", cxxopts::value<int>(), "(int)")

        ("h,help", "Print help");
        

    auto result = options.parse(argc, argv);

    if (result.count("help") || result.arguments().empty()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::cout << "Using device: " << result["device"].as<std::string>() << std::endl;
    std::cout << "Exporting configuration to: " << result["export"].as<std::string>() << std::endl;

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

        if (result.count("uwb_mode")) {
            int uwbMode = result["uwb_mode"].as<int>();
            errorCode = handler.SetUwbMode(uwbMode);
            if (errorCode == DeviceHandler::EResult::kSuccess) {
                int uwbModeBackup = uwbMode;
                errorCode = handler.GetUwbMode(response, uwbMode);
                if (errorCode == DeviceHandler::EResult::kSuccess && uwbMode == uwbModeBackup) {
                    std::cout << "UWB Mode verified successfully." << std::endl;
                } else {
                    std::cerr << "Failed to verify UWB Mode." << std::endl;
                }
            } else {
                std::cerr << "Failed to set UWB Mode." << std::endl;
            }

        }
        
        
        if (result.count("dev_id") || result.count("dev_role") || result.count("dev_channel") || result.count("dev_rate")) {
            DeviceParameters deviceParams;
            errorCode = handler.GetCfg(response, deviceParams);
            if (errorCode == DeviceHandler::EResult::kSuccess) {
                if (result.count("dev_id")) {
                    deviceParams.id = result["dev_id"].as<int>();
                }
                if (result.count("dev_role")) {
                    deviceParams.role = result["dev_role"].as<int>();
                }
                if (result.count("dev_channel")) {
                    deviceParams.channel = result["dev_channel"].as<int>();
                }
                if (result.count("dev_rate")) {
                    deviceParams.rate = result["dev_rate"].as<int>();
                }
                errorCode = handler.SetCfg(deviceParams);
                if (errorCode == DeviceHandler::EResult::kSuccess) {
                    DeviceParameters deviceParamsBackup = deviceParams; 
                    errorCode = handler.GetCfg(response, deviceParams);
                    if (errorCode == DeviceHandler::EResult::kSuccess && std::memcmp(&deviceParams, &deviceParamsBackup, sizeof(DeviceParameters)) == 0) {
                        std::cout << "Device configuration verified successfully." << std::endl;
                    } else {
                        std::cerr << "Failed to verify device configuration." << std::endl;
                    }
                } else {
                    std::cerr << "Failed to update device configuration." << std::endl;
                }
            } else {
                std::cerr << "Failed to get current device configuration." << std::endl;
            }
        }

        if (result.count("twr_tag_cap") || result.count("twr_antenna_delay") || result.count("twr_kalman_filter_enable") ||
            result.count("twr_kalman_Q") || result.count("twr_kalman_R") || result.count("twr_correction_a") ||
            result.count("twr_correction_b") || result.count("twr_positioning_enable") || result.count("twr_positioning_dimension")) {
            TwrParameters twrParams;
            errorCode = handler.GetDev(response, twrParams);
            if (errorCode == DeviceHandler::EResult::kSuccess) {
                if (result.count("twr_tag_cap")) {
                    twrParams.tagCapacity = result["twr_tag_cap"].as<int>();
                }
                if (result.count("twr_antenna_delay")) {
                    twrParams.antennaDelay = result["twr_antenna_delay"].as<int>();
                }
                if (result.count("twr_kalman_filter_enable")) {
                    twrParams.isKalmanFilterEnabled = result["twr_kalman_filter_enable"].as<int>();
                }
                if (result.count("twr_kalman_Q")) {
                    twrParams.kalmanQ = result["twr_kalman_Q"].as<double>();
                }
                if (result.count("twr_kalman_R")) {
                    twrParams.kalmanR = result["twr_kalman_R"].as<double>();
                }
                if (result.count("twr_correction_a")) {
                    twrParams.correctionParameterA = result["twr_correction_a"].as<double>();
                    std::cout << "twr_correction_a: " << twrParams.correctionParameterA << std::endl;
                }
                if (result.count("twr_correction_b")) {
                    twrParams.correctionParameterB = result["twr_correction_b"].as<double>();
                    std::cout << "twr_correction_b: " << twrParams.correctionParameterB << std::endl;
                }
                if (result.count("twr_positioning_enable")) {
                    twrParams.isPositioningEnabled = result["twr_positioning_enable"].as<int>();
                }
                if (result.count("twr_positioning_dimension")) {
                    twrParams.positioningDimension = result["twr_positioning_dimension"].as<int>();
                }
                errorCode = handler.SetDev(twrParams);
                if (errorCode == DeviceHandler::EResult::kSuccess) {
                    TwrParameters twrParamsBackup = twrParams; 
                    errorCode = handler.GetDev(response, twrParams);
                    if (errorCode == DeviceHandler::EResult::kSuccess && std::memcmp(&twrParams, &twrParamsBackup, sizeof(TwrParameters)) == 0) {
                        std::cout << "TWR parameters verified successfully." << std::endl;
                    } else {
                        std::cerr << "Failed to verify TWR parameters." << std::endl;
                    }
                } else {
                    std::cerr << "Failed to update TWR parameters." << std::endl;
                }
            } else {
                std::cerr << "Failed to get current TWR parameters." << std::endl;
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
