
#include <chrono>
#include <span>

#include <cstddef>
#include <cstdin>

class IUart {
public:
    virtual ~IUart() = default;

    virtual void write(std::span<const std::uint8_t> data) = 0;
    virtual std::size_t read(std::span<std::uint8_t> buffer, std::chrono::milliseconds timeout) = 0;
};

