#include "sogepch.hpp"

#include "SOGE/Graphics/CameraManager.hpp"


namespace soge
{
    auto CameraManager::CreateCamera(Camera aCamera) -> eastl::pair<Camera&, Key>
    {
        const auto key = UUID::Generate();

        const auto [iter, _] = m_cameras.try_emplace(key, std::move(aCamera));
        return {iter->second, key};
    }

    Camera* CameraManager::GetCamera(const Key& aKey)
    {
        const auto iter = m_cameras.find(aKey);
        if (iter == m_cameras.end())
        {
            return nullptr;
        }

        return &iter->second;
    }

    eastl::optional<Camera> CameraManager::DestroyCamera(const Key& aKey)
    {
        const auto iter = m_cameras.find(aKey);
        if (iter == m_cameras.end())
        {
            return eastl::nullopt;
        }

        Camera camera = std::move(iter->second);
        m_cameras.erase(iter);

        return camera;
    }

    void CameraManager::Clear()
    {
        m_cameras.clear();
    }
}
