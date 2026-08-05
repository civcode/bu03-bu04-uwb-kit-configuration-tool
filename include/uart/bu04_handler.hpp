#ifndef BU04_HANDLER_HPP_
#define BU04_HANDLER_HPP_

#include "uart/iuart.hpp"

#include <string_view>

class BU04Handler {
public:
    enum class EResult {
        kSuccess,
        kTimeout,
        kUnexpectedResponse,
        // kUartError,
        kError
    };

    // enum class EResponseCode {
    //     kSuccess,
    //     kError
    // };

    // typedef struct ResponseData_ {
    //     std::string str;
    //     EResponseCode code;
    // } ResponseData;

    typedef struct DeviceConfiguration_ {
        int id;
        int role;
        int channel;
        int rate;
    } DeviceConfiguration;

    typedef struct TwrDeviceSetup_ {
        int tagCapacity;
        int antennaDelay;
        bool isKalmanFilterEnabled;
        float kalmanQ;
        float kalmanR;
        float correctionParameterA;
        float correctionParameterB;
        bool isPositioningEnabled;
        int positioningDimension;
    } TwrDeviceSetup;

    explicit BU04Handler(IUart& uart, std::chrono::milliseconds timeout = std::chrono::milliseconds(1000))
        : uart_(uart), timeout_(timeout), timePrevTx_(std::chrono::high_resolution_clock::now()) {}

    // void handleData(const char* data);

    EResult SaveCfg(std::string& response);
    EResult GetVersion(std::string& response, std::string& version);
    EResult GetCfg(std::string& response, DeviceConfiguration& deviceConfig);
    EResult GetDistance(std::string& response, float& distance);
    EResult GetDev(std::string& response, TwrDeviceSetup& setup);
    EResult GetDList(std::string& response);

    EResult SetCfg(const DeviceConfiguration& deviceConfig);
    EResult SetDev(const TwrDeviceSetup& setup);

    void PrintHex(const std::string& str);
    void PrintAllChar(const std::string_view text);

private:
    constexpr static std::size_t kMaxResponseSize = 1024;
    // constexpr static std::chrono::milliseconds kUartTxInterval = std::chrono::milliseconds(500);
    constexpr static std::chrono::milliseconds kUartWaitTime = std::chrono::milliseconds(100);

    std::chrono::high_resolution_clock::time_point timePrevTx_;

    IUart& uart_;
    std::chrono::milliseconds timeout_;

    // EResult ParseResponse(const std::string& response, ResponseData& responseData);
    EResult ParseResponse(std::string& response);

    EResult HandleComm(const std::string& command, std::string& response);

    EResult ExtractDataString(const std::string& str, const std::string& prefix, 
        const std::string delimiter, std::string& data);

    inline void RemoveTerminator(std::string& str) {
        str.erase(str.find_last_not_of(" \r\n") + 1);
    }

};

#endif // BU04_HANDLER_HPP_