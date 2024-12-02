#ifndef SOGE_SYSTEM_WINDOW_HPP
#define SOGE_SYSTEM_WINDOW_HPP


namespace soge
{
    struct WindowDesc
    {
        eastl::wstring m_title;
        std::uint32_t m_width;
        std::uint32_t m_height;

        WindowDesc(
            const eastl::wstring& aWindowTitle = L"SOGE",
            std::uint32_t aWindowWidth = 1280,
            std::uint32_t aWindowHeight = 720)
            : m_title(aWindowTitle), m_width(aWindowWidth), m_height(aWindowHeight)
        {
        }
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void* GetNativeHandler() const = 0;
        virtual eastl::wstring GetTitle() const = 0;
        virtual std::uint32_t GetHeight() const = 0;
        virtual std::uint32_t GetWidth() const = 0;

    public:
        static Window* Create(WindowDesc aWindowDescriptor = WindowDesc());

    };
}

#endif // !SOGE_SYSTEM_WINDOW_HPP
