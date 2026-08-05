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

    typedef struct DeviceConfiguration_ {
        int id;
        int role;
        int channel;
        int rate;
    } DeviceConfiguration;

    typedef struct SensorData_ {
        float accX;
        float accY;
        float accZ;
        float angle;
    } SensorData;

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

    typedef struct PdoaConfiguration_ {
        int dlist;
        int klist;
        int net;
        int anchId;
        int rate;
        bool isFilterEnabled;
        int userCmd;
        int pdoaOffset;
        int rngOffset;
    } PdoaConfiguration;

    explicit BU04Handler(IUart& uart, std::chrono::milliseconds timeout = std::chrono::milliseconds(1000))
        : uart_(uart), timeout_(timeout), timePrevTx_(std::chrono::high_resolution_clock::now()) {}

    // void handleData(const char* data);
    EResult Save(std::string& response);
    EResult Restart(std::string& response);
    EResult Restore(std::string& response);

    EResult GetAt(std::string& response);
    EResult GetVer(std::string& response, std::string& version);
    EResult GetWorkMode(std::string& response, int& workMode);
    EResult GetCfg(std::string& response, DeviceConfiguration& deviceConfig);
    EResult GetSensor(std::string& response, SensorData& sensorData);
    EResult GetDistance(std::string& response, float& distance);
    EResult GetDev(std::string& response, TwrDeviceSetup& setup);
    EResult GetDeca(std::string& response);
    EResult GetDList(std::string& response);
    EResult GetKList(std::string& response);
    EResult GetPdoaCfg(std::string& response, PdoaConfiguration& pdoaCfg);
    EResult GetUwbMode(std::string& response, int& uwbMode);

    EResult TestLed(std::string& response, int state);
    EResult TestOled(std::string& response);


    EResult SetWorkMode(int workMode);
    EResult SetCfg(const DeviceConfiguration& deviceConfig);
    EResult SetDev(const TwrDeviceSetup& setup);
    EResult SetPdoaOffset(int offset); // angle offset in deg
    EResult SetRngOffset(int offset); // distance offset in mm

    EResult SetUwbMode(int uwbMode);

    void PrintHex(const std::string& str);
    void PrintAllChar(const std::string_view text);

private:
    constexpr static std::size_t kMaxResponseSize = 1024;
    constexpr static std::chrono::milliseconds kUartWaitTime = std::chrono::milliseconds(100);

    std::chrono::high_resolution_clock::time_point timePrevTx_;

    IUart& uart_;
    std::chrono::milliseconds timeout_;

    // EResult ParseResponse(const std::string& response, ResponseData& responseData);
    EResult ExtractErrorCode(const std::string& response); 
    EResult ParseResponse(std::string& response);

    EResult HandleComm(const std::string& command, std::string& response, bool returnRawResponse=false);

    EResult ExtractDataString(const std::string& str, const std::string& prefix, 
        const std::string delimiter, std::string& data);

    inline void RemoveTerminator(std::string& str) {
        str.erase(str.find_last_not_of(" \r\n") + 1);
    }

};

#endif // BU04_HANDLER_HPP_