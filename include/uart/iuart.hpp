
#include <chrono>
#include <span>

#include <cstddef>
#include <cstdint>

class IUart {
public:
    virtual ~IUart() = default;

    virtual std::size_t read(std::uint8_t* buffer, std::size_t capacity, 
                             std::chrono::milliseconds firstByteTimeout=std::chrono::milliseconds(500),
                             std::chrono::milliseconds interByteTimeout=std::chrono::milliseconds(50)) = 0;
    virtual bool write(const std::uint8_t* data, std::size_t size) = 0;

    virtual std::size_t readText(std::string& text, std::size_t maxBytes, 
                                 std::chrono::milliseconds firstByteTimeout=std::chrono::milliseconds(500),
                                 std::chrono::milliseconds interByteTimeout=std::chrono::milliseconds(50)) {
        text.clear();
        text.resize(maxBytes);

        const std::size_t bytesRead = read(
            reinterpret_cast<std::uint8_t*>(text.data()),
            maxBytes,
            firstByteTimeout,
            interByteTimeout);

        text.resize(bytesRead);
        return bytesRead;
    }

    virtual bool writeText(std::string_view text) {
        return write(reinterpret_cast<const std::uint8_t*>(text.data()), text.size());
    };
};

