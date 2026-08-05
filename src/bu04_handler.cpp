#include "uart/bu04_handler.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using EResult = BU04Handler::EResult;

EResult BU04Handler::GetCfg(std::string & response, DeviceConfiguration& deviceConfig) 
{
    std::cout << "GetCfg() called" << std::endl;
    
    EResult result;
    const std::string command = "AT+GETCFG\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;

    try {
        if (ExtractDataString(received, "getcfg ID:", ",", data) == EResult::kSuccess) {
            deviceConfig.id = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "Role:", ",", data) == EResult::kSuccess) {
            deviceConfig.role = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "CH:", ",", data) == EResult::kSuccess) {
            deviceConfig.channel = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "Rate:", ",", data) == EResult::kSuccess) {
            deviceConfig.rate = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
    } catch (const std::invalid_argument&) {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::SaveCfg(std::string& response)
{
    std::cout << "SaveCfg() called" << std::endl;

    EResult result;
    const std::string command = "AT+SAVE\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;
   
    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetVersion(std::string& response, std::string& version)
{
    std::cout << "GetVersion() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETVER\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;
    if (ExtractDataString(received, "getver software:", ",", data) == EResult::kSuccess) {
        version = data;
    } else {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetDistance(std::string &response, float &distance)
{
    std::cout << "GetDistance() called" << std::endl;

    EResult result;
    const std::string command = "AT+DISTANCE\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;

    try {
        if (ExtractDataString(received, "distance: ", ",", data) == EResult::kSuccess) {
            distance = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
    } catch (const std::invalid_argument&) {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetDev(std::string &response, TwrDeviceSetup& setup)
{
    std::cout << "GetDev() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETDEV\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;

    try {
        if (ExtractDataString(received, "getdev cap:", " ", data) == EResult::kSuccess) {
            setup.tagCapacity = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "anndelay:", ",", data) == EResult::kSuccess) {
            setup.antennaDelay = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "kalman_enable:", ",", data) == EResult::kSuccess) {
            setup.isKalmanFilterEnabled = (data == "1");
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "kalman_Q:", ",", data) == EResult::kSuccess) {
            setup.kalmanQ = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "kalman_R:", ",", data) == EResult::kSuccess) {
            setup.kalmanR = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "para_a:", ",", data) == EResult::kSuccess) {
            setup.correctionParameterA = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "para_b:", ",", data) == EResult::kSuccess) {
            setup.correctionParameterB = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "pos_enable:", ",", data) == EResult::kSuccess) {
            setup.isPositioningEnabled = (data == "1");
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "pos_dimen:", ",", data) == EResult::kSuccess) {
            setup.positioningDimension = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
    } catch (const std::invalid_argument&) {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::ParseResponse(std::string& response)
{
    std::cout << "ParseResponse() called" << std::endl;
    PrintAllChar(response);

    EResult result;

    // Extract the response code
    if (response.find("OK") != std::string::npos) {
        result = EResult::kSuccess;
    } else if (response.find("ERR") != std::string::npos) {
        result = EResult::kError;
    } else {
        result = EResult::kUnexpectedResponse;
    }
    // PrintAllChar(response);
    // std::cout << "Response code: " << (result == EResult::kSuccess ? "OK" : "ERR") << std::endl;

    // Prune leading special characters
    while (!response.empty() &&
              (response.front() == '\r' || response.front() == '\n' || response.front() == '\0')) {
          response.erase(response.begin());
    }
    // std::cout << "PrintAllChar(response)" << std::endl;

    // Extract the remaining response string
    if (response.find("\r\n") != std::string::npos) {
        response = response.substr(0, response.find("\r\n"));
        response.append(",");
    }
    // std::cout << "ParseRespnse():" << std::endl;
    // std::cout << (result == EResult::kSuccess ? "OK" : "ERR") << std::endl;
    // std::cout << std::endl; 

    PrintAllChar(response);
    std::cout << "Response code: " << (result == EResult::kSuccess ? "OK" : "ERR") << std::endl;

    return result;
}

EResult BU04Handler::HandleComm(const std::string &command, std::string &response)
{
    std::cout << "HandleComm() called" << std::endl;
    PrintAllChar(command);
    uart_.writeText(command);

    std::string received;
    const std::size_t bytesRead = uart_.readText(received, 256, timeout_);
    // PrintAllChar(received);

    if (bytesRead == 0) {
        return EResult::kTimeout;
    }

    EResult result;
    result = ParseResponse(received);
    if (result == EResult::kSuccess) {
        response = received;
    } else {
        std::cout << "Unexpected response: " << received << std::endl;
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::ExtractDataString(const std::string &str, const std::string &prefix,
                                       const std::string delimiter, std::string &data)
{
    const auto pos = str.find(prefix);
    const auto endPos = str.find(delimiter, pos + prefix.length());

    if (pos != std::string::npos && endPos != std::string::npos) {
        data = str.substr(pos + prefix.length(), endPos - (pos + prefix.length()));
        // std::cout << "Extracted data: " << data << std::endl;
        return EResult::kSuccess;
    }
    std::cout << "Failed to extract data with prefix: " << prefix << std::endl;
    return EResult::kUnexpectedResponse;
}

EResult BU04Handler::SetCfg(const DeviceConfiguration &deviceConfig)
{
    // uart_.writeText(command);

    // std::string received;
    // const std::size_t bytesRead = uart_.readText(received, 256, timeout_);
    // PrintAllChar(received);
    // // std::cout << "Received response: " << received << std::endl;

    // if (bytesRead == 0) {
    //     return EResult::kTimeout;
    // }
    
    EResult result;
    const std::string command = "AT+SETCFG=" + std::to_string(deviceConfig.id) + "," +
                                std::to_string(deviceConfig.role) + "," +
                                std::to_string(deviceConfig.channel) + "," +
                                std::to_string(deviceConfig.rate) + "\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}
EResult BU04Handler::SetDev(const TwrDeviceSetup &setup)
{
    const std::string command = "AT+SETDEV=" + std::to_string(setup.tagCapacity) + "," +
                                std::to_string(setup.antennaDelay) + "," +
                                std::to_string(setup.isKalmanFilterEnabled ? 1 : 0) + "," +
                                std::to_string(setup.kalmanQ) + "," +
                                std::to_string(setup.kalmanR) + "," +
                                std::to_string(setup.correctionParameterA) + "," +
                                std::to_string(setup.correctionParameterB) + "," +
                                std::to_string(setup.isPositioningEnabled ? 1 : 0) + "," +
                                std::to_string(setup.positioningDimension) + "\r\n";

    std::cout << "Sending command: " << command << std::endl;
    // return EResult::kSuccess;

    uart_.writeText(command);
    std::string received;
    const std::size_t bytesRead = uart_.readText(received, 256, timeout_);
    // RemoveTerminator(received);
    // PrintHex(received);
    PrintAllChar(received);
    // std::cout << received << std::flush;
    std::cout << "Received response: " << received << std::endl;

    if (bytesRead == 0) {
        return EResult::kTimeout;
    }

    return EResult();
}
void BU04Handler::PrintHex(const std::string &str)
{
    for (const unsigned char c : str) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;
}
void BU04Handler::PrintAllChar(const std::string_view text)
{
        for (const unsigned char ch : text) {
        switch (ch) {
            case '\r':
                std::cout << "\\r";
                break;
            case '\n':
                std::cout << "\\n";
                break;
            case '\t':
                std::cout << "\\t";
                break;
            case '\0':
                std::cout << "\\0";
                break;
            case '\b':
                std::cout << "\\b";
                break;
            case '\x1b':
                std::cout << "\\x1b";
                break;
            default:
                if (std::isprint(ch)) {
                    std::cout << static_cast<char>(ch);
                } else {
                    std::cout << "\\x"
                              << std::hex
                              << std::setw(2)
                              << std::setfill('0')
                              << static_cast<unsigned int>(ch)
                              << std::dec;
                }
                break;
        }
    }

    std::cout << '\n';
}