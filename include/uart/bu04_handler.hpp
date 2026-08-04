#ifndef BU04_HANDLER_HPP_
#define BU04_HANDLER_HPP_

#include "uart/iuart.hpp"

class BU04Handler {
public:
    enum class EResult {
        kSuccess,
        kTimeout,
        kUnexpectedResponse,
        kError
    };

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
        : uart_(uart), timeout_(timeout) {}

    // void handleData(const char* data);

    EResult GetVersion(std::string& version);
    EResult GetDev(std::string& devInfo, TwrDeviceSetup& setup);

private:
    IUart& uart_;
    std::chrono::milliseconds timeout_;

    EResult ExtractDataString(const std::string& str, const std::string& prefix, 
        const std::string delimiter, std::string& data);
};

#endif // BU04_HANDLER_HPP_