#ifndef SOGE_INPUT_DEVICE_INPUTDEVICE_HPP
#define SOGE_INPUT_DEVICE_INPUTDEVICE_HPP


namespace soge
{
    enum class DeviceType : std::uint8_t
    {
        // NOLINTBEGIN(readability-identifier-naming) reason: naming errors can occur
        DeviceType_Keyboard,
        DeviceType_Mouse,
        DeviceType_Gamepad,
        // NOLINTEND(readability-identifier-naming) reason: naming errors can occur
    };

    class InputDevice
    {
    private:
        eastl::string_view m_deviceName;
        DeviceType m_deviceType;

    public:
        InputDevice(eastl::string_view aDeviceName, DeviceType aDeviceType);

        explicit InputDevice(const InputDevice&) = delete;
        InputDevice& operator=(const InputDevice&) = delete;

        explicit InputDevice(InputDevice&&) noexcept = default;
        InputDevice& operator=(InputDevice&&) noexcept = default;

        virtual ~InputDevice() = default;

        [[nodiscard]]
        eastl::string_view GetDeviceName() const;
        [[nodiscard]]
        eastl::string ToString() const;

        [[nodiscard]]
        DeviceType GetDeviceType() const;
    };
}

#endif // SOGE_INPUT_DEVICE_INPUTDEVICE_HPP
