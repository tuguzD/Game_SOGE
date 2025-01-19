#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/Deferred/DeferredRenderGraph.hpp>
#include <SOGE/Graphics/Entities/AmbientLightEntity.hpp>
#include <SOGE/Graphics/Entities/DirectionalLightEntity.hpp>
#include <SOGE/Graphics/Entities/GeometryEntity.hpp>
#include <SOGE/Graphics/Entities/PointLightEntity.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Graphics/Primitives/Box.hpp>
#include <SOGE/Graphics/Primitives/Sphere.hpp>
#include <SOGE/Math/Camera.hpp>
#include <SOGE/Window/WindowModule.hpp>

#undef CreateWindow


namespace soge_game
{
    Game::Game(AccessTag&& aTag) : Engine(std::move(aTag))
    {
        SOGE_APP_INFO_LOG("Initialize game...");
    }

    Game::~Game()
    {
        SOGE_APP_INFO_LOG("Destroy game...");

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Game::Load(AccessTag)
    {
        PushLayer(new MainGameLayer());

        auto& container = GetDependencyContainer();
        const auto eventModule = GetModule<soge::EventModule>();
        const auto inputModule = GetModule<soge::InputModule>();
        const auto windowModule = GetModule<soge::WindowModule>();
        const auto graphicsModule = GetModule<soge::GraphicsModule>();

        const auto [window, windowUuid] = windowModule->CreateWindow();
        SOGE_INFO_LOG(R"(Created window "{}" of width {} and height {} with UUID {})",
                      soge::EAToNarrow(window.GetTitle()).c_str(), window.GetWidth(), window.GetHeight(),
                      windowUuid.str());

        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::DeferredRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        constexpr std::size_t gridSize = 3;
        constexpr glm::vec3 gridOffset{-0.5f * (gridSize - 1)};
        for (std::size_t i = 0; i < gridSize; ++i)
        {
            for (std::size_t j = 0; j < gridSize; ++j)
            {
                for (std::size_t k = 0; k < gridSize; ++k)
                {
                    auto box = soge::CreateBox(container.Provide<soge::GraphicsCore>(),
                                               container.Provide<soge::GeometryGraphicsPipeline>());
                    const auto [entity, entityUuid] =
                        graphicsModule->GetEntityManager().CreateEntity<soge::GeometryEntity>(std::move(box));
                    SOGE_INFO_LOG(R"(Created box ({}, {}, {}) with UUID {})", i, j, k, entityUuid.str());

                    const auto x = static_cast<float>(i);
                    const auto y = static_cast<float>(j);
                    const auto z = static_cast<float>(k);
                    entity.GetTransform() = soge::Transform{
                        .m_position = glm::vec3{x, y, z} + gridOffset,
                        // .m_rotation = glm::quat{glm::vec3{0.0f, glm::radians(45.0f), 0.0f}},
                        .m_scale = glm::vec3{0.5f},
                    };
                }
            }
        }

        auto sphere = soge::CreateSphere(container.Provide<soge::GraphicsCore>(),
                                         container.Provide<soge::GeometryGraphicsPipeline>());
        const auto [entity, entityUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::GeometryEntity>(std::move(sphere));
        SOGE_INFO_LOG(R"(Created sphere with UUID {})", entityUuid.str());
        entity.GetTransform() = soge::Transform{
            .m_position = glm::vec3{2.0f, 0.0f, 0.0f},
            .m_scale = glm::vec3{0.5f},
        };

        const auto [ambientLightEntity1, ambientLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::AmbientLightEntity>(
                container.Provide<soge::AmbientLightEntity>());
        SOGE_INFO_LOG(R"(Created ambient light entity with UUID {})", ambientLightEntityUuid1.str());
        ambientLightEntity1.GetIntensity() = 0.01f;

        const auto [ambientLightEntity2, ambientLightEntityUuid2] =
            graphicsModule->GetEntityManager().CreateEntity<soge::AmbientLightEntity>(
                container.Provide<soge::AmbientLightEntity>());
        SOGE_INFO_LOG(R"(Created ambient light entity with UUID {})", ambientLightEntityUuid2.str());
        ambientLightEntity2.GetIntensity() = 0.05f;
        ambientLightEntity2.GetColor() = glm::vec3{1.0f, 0.0f, 0.0f};

        const auto [directionalLightEntity1, directionalLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_INFO_LOG(R"(Created directional light entity with UUID {})", directionalLightEntityUuid1.str());
        const soge::Transform directionalLightTransform1{
            .m_rotation = glm::quat{glm::vec3{glm::radians(45.0f), glm::radians(45.0f), 0.0f}},
        };
        directionalLightEntity1.GetDirection() = directionalLightTransform1.Forward();

        const auto [directionalLightEntity2, directionalLightEntityUuid2] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_INFO_LOG(R"(Created directional light entity with UUID {})", directionalLightEntityUuid2.str());
        const soge::Transform directionalLightTransform2{
            .m_rotation = glm::quat{glm::vec3{glm::radians(45.0f), -glm::radians(45.0f), 0.0f}},
        };
        directionalLightEntity2.GetIntensity() = 0.5f;
        directionalLightEntity2.GetColor() = glm::vec3{0.0f, 1.0f, 0.0f};
        directionalLightEntity2.GetDirection() = directionalLightTransform2.Forward();

        const auto [pointLightEntity1, pointLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::PointLightEntity>(
                container.Provide<soge::PointLightEntity>());
        SOGE_INFO_LOG(R"(Created point light entity with UUID {})", pointLightEntityUuid1.str());
        pointLightEntity1.GetAttenuation().m_linearFactor = glm::vec3{1.0f};
        pointLightEntity1.GetAttenuation().m_quadraticFactor = glm::vec3{100.0f};

        const auto [camera, cameraUuid] = graphicsModule->GetCameraManager().CreateCamera({
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f,
            .m_farPlane = 100.0f,
            .m_transform = soge::Transform{.m_position = glm::vec3{0.0f, 0.0f, -2.0f}},
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });
        SOGE_INFO_LOG(R"(Created camera with UUID {})", cameraUuid.str());

        const auto [viewport, viewportUuid] = graphicsModule->GetViewportManager().CreateViewport({
            .m_viewport = {static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight())},
            .m_cameraId = cameraUuid,
        });
        SOGE_INFO_LOG(R"(Created viewport with UUID {})", viewportUuid.str());

        // share state between two lambdas
        auto mouseDeltaX = soge::CreateShared<float>(0.0f);
        auto mouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [mouseDeltaX, mouseDeltaY, inputModule](const soge::MouseMovedEvent& aEvent) {
            if (!inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                return;
            }
            *mouseDeltaX = aEvent.GetXOffset();
            *mouseDeltaY = aEvent.GetYOffset();
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        auto mouseWheel = [&pointLightEntity1](const soge::MouseWheelEvent& aEvent) {
            pointLightEntity1.GetPosition() += glm::vec3{0.0f, aEvent.GetXOffset() * 0.1f, 0.0f};
        };
        eventModule->PushBack<soge::MouseWheelEvent>(mouseWheel);

        float cameraPitch{}, cameraYaw{};
        constexpr float cameraSensitivity = 0.005f;
        auto update = [=, &camera](const soge::UpdateEvent& aEvent) mutable {
            {
                const float x = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::D)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::A));
                const float y = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::SpaceBar)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::LeftShift));
                const float z = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::W)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::S));
                const auto direction =
                    camera.m_transform.Right() * x + camera.m_transform.Up() * y + camera.m_transform.Forward() * z;
                camera.m_transform.m_position += direction * aEvent.GetDeltaTime();
            }

            cameraYaw += *mouseDeltaX * cameraSensitivity;
            cameraPitch += *mouseDeltaY * cameraSensitivity;
            camera.m_transform.m_rotation = glm::quat{glm::vec3{cameraPitch, cameraYaw, 0.0f}};

            *mouseDeltaX = 0.0f;
            *mouseDeltaY = 0.0f;
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
