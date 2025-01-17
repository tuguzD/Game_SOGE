#ifndef SOGE_GRAPHICS_GRAPHICSENTITY_HPP
#define SOGE_GRAPHICS_GRAPHICSENTITY_HPP


namespace soge
{
    class GraphicsEntity
    {
    public:
        constexpr explicit GraphicsEntity() noexcept = default;

        constexpr GraphicsEntity(const GraphicsEntity&) = delete;
        constexpr GraphicsEntity& operator=(const GraphicsEntity&) = delete;

        constexpr GraphicsEntity(GraphicsEntity&&) noexcept = default;
        constexpr GraphicsEntity& operator=(GraphicsEntity&&) noexcept = default;

        constexpr virtual ~GraphicsEntity() noexcept = default;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSENTITY_HPP
