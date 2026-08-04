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

    explicit BU04Handler(IUart& uart) : uart_(uart) {}

    void handleData(const char* data);

    void GetVersion(std::string& version, std::chrono::milliseconds timeout);

private:
    IUart& uart_;
};

#endif // BU04_HANDLER_HPP_