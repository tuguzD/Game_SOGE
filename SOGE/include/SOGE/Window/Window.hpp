#ifndef SOGE_WINDOW_WINDOW_HPP
#define SOGE_WINDOW_WINDOW_HPP


namespace soge
{
    struct WindowDesc
    {
        eastl::wstring m_title;
        std::uint32_t m_width;
        std::uint32_t m_height;

        explicit WindowDesc(eastl::wstring aTitle = L"SOGE", std::uint32_t aWidth = 1280, std::uint32_t aHeight = 720);
    };

    class Window
    {
    public:
        explicit Window(const WindowDesc& aWindowDesc);

        explicit Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        explicit Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        virtual ~Window() = default;

        [[nodiscard]]
        virtual void* GetNativeHandler() const = 0;

        [[nodiscard]]
        virtual eastl::wstring GetTitle() const = 0;
        [[nodiscard]]
        virtual std::uint32_t GetHeight() const = 0;
        [[nodiscard]]
        virtual std::uint32_t GetWidth() const = 0;
    };
}

#endif // SOGE_WINDOW_WINDOW_HPP
