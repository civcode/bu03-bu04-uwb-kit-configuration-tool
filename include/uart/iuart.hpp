
#include <chrono>
#include <span>

#include <cstddef>
#include <cstdint>

class IUart {
public:
    virtual ~IUart() = default;

    // virtual std::size_t read(std::uint8_t* buffer, std::size_t capacity, std::chrono::milliseconds timeout) = 0;
    virtual std::size_t read(std::uint8_t* buffer, std::size_t capacity, std::chrono::milliseconds firstByteTimeout,
                             std::chrono::milliseconds interByteTimeout=std::chrono::milliseconds(50)) = 0;
    // virtual std::size_t readText(std::string& text, std::size_t maxBytes, std::chrono::milliseconds timeout) = 0;
    virtual bool writeBytes(const std::uint8_t* data, std::size_t size) = 0;
    virtual bool writeText(std::string_view text) {
        return writeBytes(reinterpret_cast<const std::uint8_t*>(text.data()), text.size());
    };
};

