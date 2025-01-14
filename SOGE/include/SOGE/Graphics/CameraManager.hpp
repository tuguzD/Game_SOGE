#ifndef SOGE_GRAPHICS_CAMERAMANAGER_HPP
#define SOGE_GRAPHICS_CAMERAMANAGER_HPP

#include "SOGE/Math/Camera.hpp"
#include "SOGE/Utils/UUID.hpp"

#include <EASTL/hash_map.h>
#include <EASTL/optional.h>


namespace soge
{
    class CameraManager
    {
    private:
        using Key = UUIDv4::UUID;

        using Cameras = eastl::hash_map<Key, Camera>;
        Cameras m_cameras;

    public:
        template <typename... Args>
        [[nodiscard]]
        eastl::pair<Camera&, Key> CreateCamera(Args&&... args);

        [[nodiscard]]
        eastl::pair<Camera&, Key> CreateCamera(Camera aCamera);

        [[nodiscard]]
        Camera* GetCamera(const Key& aKey);

        eastl::optional<Camera> DestroyCamera(const Key& aKey);

        void Clear();
    };

    template <typename... Args>
    auto CameraManager::CreateCamera(Args&&... args) -> eastl::pair<Camera&, Key>
    {
        Camera camera{std::forward<Args>(args)...};
        return CreateCamera(std::move(camera));
    }
}

#endif // SOGE_GRAPHICS_CAMERAMANAGER_HPP
