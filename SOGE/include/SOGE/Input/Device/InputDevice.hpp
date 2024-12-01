#ifndef SOGE_INPUT_INPUTDEVICE_HPP
#define SOGE_INPUT_INPUTDEVICE_HPP


namespace soge
{
    enum class DeviceType
    {
        DeviceType_Keyboard,
        DeviceType_Mouse,
        DeviceType_Gamepad
    };

    class InputDevice
    {
    private:
        eastl::string_view m_deviceName;
        DeviceType m_deviceType;

    public:
        InputDevice(eastl::string_view aDeviceName, DeviceType aDeviceType);
        virtual ~InputDevice() = default;

        eastl::string_view GetDeviceName() const;
        eastl::string ToString() const;
        DeviceType GetDeviceType() const;

    };
}

#endif // !SOGE_INPUT_INPUTDEVICE_HPP
