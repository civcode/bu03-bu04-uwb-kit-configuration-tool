#include "uart/bu04_handler.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <thread>

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

EResult BU04Handler::GetAt(std::string& response)
{
    std::cout << "GetAt() called" << std::endl;

    EResult result;
    const std::string command = "AT\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}




EResult BU04Handler::Save(std::string& response)
{
    std::cout << "Save() called" << std::endl;

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

EResult BU04Handler::Restart(std::string& response)
{
    std::cout << "Restart() called" << std::endl;

    EResult result;
    const std::string command = "AT+RESTART\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::Restore(std::string& response)
{
    std::cout << "Restore() called" << std::endl;

    EResult result;
    const std::string command = "AT+RESTORE\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetVer(std::string& response, std::string& version)
{
    std::cout << "GetVer() called" << std::endl;

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

EResult BU04Handler::GetWorkMode(std::string& response, int& workMode)
{
    std::cout << "GetWorkMode() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETWORKMODE\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;
    if (ExtractDataString(received, "workmode:", ",", data) == EResult::kSuccess) {
        workMode = std::stoi(data);
    } else {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetSensor(std::string& response, SensorData& sensorData)
{
    std::cout << "GetSensor() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETSENSOR\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;
    try {
        if (ExtractDataString(received, "accX:", ",", data) == EResult::kSuccess) {
            sensorData.accX = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "accY:", ",", data) == EResult::kSuccess) {
            sensorData.accY = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "accZ:", ",", data) == EResult::kSuccess) {
            sensorData.accZ = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "angle:", ",", data) == EResult::kSuccess) {
            sensorData.angle = std::stof(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
    } catch (const std::invalid_argument&) {
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
        if (ExtractDataString(received, "cap:", " ", data) == EResult::kSuccess) {
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

EResult BU04Handler::GetDeca(std::string &response)
{
    std::cout << "GetDeca() called" << std::endl;

    EResult result;
    const std::string command = "AT+DECA$\r\n";
    std::string received;
    result = HandleComm(command, received, true);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }
    result = ExtractErrorCode(response);

    return result;
}

EResult BU04Handler::GetDList(std::string &response)
{
    std::cout << "GetDList() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETDLIST\r\n";
    std::string received;
    result = HandleComm(command, received, true);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }
    result = ExtractErrorCode(response);

    return EResult::kSuccess;
}

EResult BU04Handler::GetKList(std::string &response)
{
    std::cout << "GetKList() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETKLIST\r\n";
    std::string received;
    result = HandleComm(command, received, true);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }
    result = ExtractErrorCode(response);

    return EResult::kSuccess;
}

EResult BU04Handler::GetPdoaCfg(std::string &response, PdoaConfiguration &pdoaCfg)
{
    std::cout << "GetPdoaCfg() called" << std::endl;

    EResult result;
    const std::string command = "AT+PDOAGETCFG\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;

    try {
        if (ExtractDataString(received, "Dlist:", " ", data) == EResult::kSuccess) {
            pdoaCfg.dlist = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "KList:", ",", data) == EResult::kSuccess) {
            pdoaCfg.klist = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "Net:", ",", data) == EResult::kSuccess) {
            pdoaCfg.net = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "AncID:", ",", data) == EResult::kSuccess) {
            pdoaCfg.anchId = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "Rate:", ",", data) == EResult::kSuccess) {
            pdoaCfg.rate = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "Filter:", ",", data) == EResult::kSuccess) {
            pdoaCfg.isFilterEnabled = (data == "1");
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "UserCmd:", ",", data) == EResult::kSuccess) {
            pdoaCfg.userCmd = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "pdoaOffset:", ",", data) == EResult::kSuccess) {
            pdoaCfg.pdoaOffset = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
        if (ExtractDataString(received, "rngOffset:", ",", data) == EResult::kSuccess) {
            pdoaCfg.rngOffset = std::stoi(data);
        } else {
            return EResult::kUnexpectedResponse;
        }
    } catch (const std::invalid_argument&) {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::GetUwbMode(std::string &response, int &uwbMode)
{
    std::cout << "GetUwbMode() called" << std::endl;

    EResult result;
    const std::string command = "AT+GETUWBMODE\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    std::string data;
    if (ExtractDataString(received, "twr_pdoa_mode:", ",", data) == EResult::kSuccess) {
        uwbMode = std::stoi(data);
    } else {
        return EResult::kUnexpectedResponse;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::ExtractErrorCode(const std::string& response)
{
    if (response.find("OK") != std::string::npos) {
        return EResult::kSuccess;
    } else if (response.find("ERR") != std::string::npos) {
        return EResult::kError;
    } else {
        return EResult::kUnexpectedResponse;
    }
}

EResult BU04Handler::ParseResponse(std::string& response)
{
    std::cout << "ParseResponse() called" << std::endl;
    // PrintAllChar(response);

    EResult result;

    // Extract the response code
    // if (response.find("OK") != std::string::npos) {
    //     result = EResult::kSuccess;
    // } else if (response.find("ERR") != std::string::npos) {
    //     result = EResult::kError;
    // } else {
    //     result = EResult::kUnexpectedResponse;
    // }
    result = ExtractErrorCode(response);
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

    std::cout << "Response string: ";
    PrintAllChar(response);
    std::cout << "Response code: " << (result == EResult::kSuccess ? "OK" : "ERR") << std::endl;

    return result;
}

EResult BU04Handler::HandleComm(const std::string &command, std::string &response, bool returnRawResponse)
{
    std::cout << "HandleComm() called" << std::endl;

    // Purge uart buffers
    // uart_.writeText("AT\r\n");
    // std::string dump;
    // uart_.readText(dump, 256, std::chrono::milliseconds(100)); 
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Wait if the previous command was sent too recently
    const auto now = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - timePrevTx_);
    if (elapsed < std::chrono::milliseconds(kUartWaitTime)) {
        std::cout << "Waiting for UART" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(kUartWaitTime));
    }
    timePrevTx_ = std::chrono::high_resolution_clock::now();


    PrintAllChar(command);
    uart_.writeText(command);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string received;
    const std::size_t bytesRead = uart_.readText(received, kMaxResponseSize, timeout_);
    // PrintAllChar(received);

    if (bytesRead == 0) {
        return EResult::kTimeout;
    }

    if (returnRawResponse) {
        response = received;
        return EResult::kSuccess;
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

EResult BU04Handler::AddTag(std::string& response, const TagParameters& tagParams)
{
    std::cout << "AddTag() called" << std::endl;

    EResult result;
    const std::string command = "AT+ADDTAG=" + tagParams.a64 + "," +
                                std::to_string(tagParams.a16) + "," +
                                std::to_string(tagParams.F) + "," +
                                std::to_string(tagParams.S) + "," +
                                std::to_string(tagParams.M) + "\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::DelTag(std::string& response, const std::string& a64)
{
    std::cout << "DelTag() called" << std::endl;

    EResult result;
    const std::string command = "AT+DELTAG=" + a64 + "\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}


EResult BU04Handler::TestLed(std::string& response, int state)
{
    std::cout << "TestLed() called" << std::endl;

    EResult result;
    const std::string command = "AT+TESTLED\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::TestOled(std::string& response)
{
    std::cout << "TestOled() called" << std::endl;

    EResult result;
    const std::string command = "AT+TESTOLED\r\n";
    std::string received;
    result = HandleComm(command, received);
    response = received;

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::SetWorkMode(int workMode)
{
    std::cout << "SetWorkMode() called" << std::endl;

    EResult result;
    const std::string command = "AT+SETWORKMODE=" + std::to_string(workMode) + "\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::SetDev(const TwrDeviceSetup &setup)
{
    std::cout << "SetDev() called" << std::endl;

    const std::string command = "AT+SETDEV=" + std::to_string(setup.tagCapacity) + "," +
                                std::to_string(setup.antennaDelay) + "," +
                                std::to_string(setup.isKalmanFilterEnabled ? 1 : 0) + "," +
                                std::to_string(setup.kalmanQ) + "," +
                                std::to_string(setup.kalmanR) + "," +
                                std::to_string(setup.correctionParameterA) + "," +
                                std::to_string(setup.correctionParameterB) + "," +
                                std::to_string(setup.isPositioningEnabled ? 1 : 0) + "," +
                                std::to_string(setup.positioningDimension) + "\r\n";

    // std::cout << "Sending command: " << command << std::endl;
    // return EResult::kSuccess;

    uart_.writeText(command);
    std::string received;
    const std::size_t bytesRead = uart_.readText(received, kMaxResponseSize, timeout_);
    // RemoveTerminator(received);
    // PrintHex(received);
    // PrintAllChar(received);
    // std::cout << received << std::flush;
    // std::cout << "Received response: " << received << std::endl;

    if (bytesRead == 0) {
        return EResult::kTimeout;
    }

    EResult result;
    result = ParseResponse(received);

    return result;
}

EResult BU04Handler::SetPdoaOffset(int offset)
{
    std::cout << "SetPdoaOffset() called" << std::endl;

    EResult result;
    const std::string command = "AT+PDOAOFF=" + std::to_string(offset) + "\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::SetRngOffset(int offset)
{
    std::cout << "SetRngOffset() called" << std::endl;

    EResult result;
    const std::string command = "AT+RNGOFF=" + std::to_string(offset) + "\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
}

EResult BU04Handler::SetUwbMode(int uwbMode)
{
    std::cout << "SetUwbMode() called" << std::endl;

    EResult result;
    const std::string command = "AT+SETUWBMODE=" + std::to_string(uwbMode) + "\r\n";
    std::string received;
    result = HandleComm(command, received);

    if (result != EResult::kSuccess) {
        return result;
    }

    return EResult::kSuccess;
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