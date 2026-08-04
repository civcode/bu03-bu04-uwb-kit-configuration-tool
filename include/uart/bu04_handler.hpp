#ifndef BU04_HANDLER_HPP_
#define BU04_HANDLER_HPP_

class BU04Handler {
public:
    enum class EResult {
        kSuccess,
        kTimeout,
        kError
    };

    BU04Handler() = default;
    ~BU04Handler() = default;

    // EResult 

    void handleData(const char* data);

private:
    IUart& uart_;
}

#endif // BU04_HANDLER_HPP_