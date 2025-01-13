#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Graphics/SimpleRenderGraph.hpp>
#include <SOGE/Graphics/TriangleEntity.hpp>
#include <SOGE/Math/Transform.hpp>
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

        auto& renderGraph = container.Provide<soge::SimpleRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        const auto [entity, entityUuid] =
            graphicsModule->CreateEntity<soge::TriangleEntity>(container.Provide<soge::TriangleEntity>());
        SOGE_INFO_LOG(R"(Created graphics triangle entity with UUID {})", entityUuid.str());

        constexpr std::array vertices{
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{-0.5f, 0.5f, 0.0f},
                .m_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
            },
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{0.5f, 0.5f, 0.0f},
                .m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
            },
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{0.0f, -0.5f, 0.0f},
                .m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
            },
        };
        entity.UpdateVertices(vertices);

        soge::Transform transform{};
        soge::Transform cameraTransform{
            .m_position = glm::vec3{0.0f, 0.0f, -1.0f},
        };
        const float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
        const glm::mat4x4 cameraProjection = glm::perspectiveLH_ZO(glm::radians(60.0f), aspectRatio, 0.01f, 100.0f);

        // share state between two lambdas
        auto mouseDeltaX = soge::CreateShared<float>(0.0f);
        auto mouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [mouseDeltaX, mouseDeltaY](const soge::MouseMovedEvent& aEvent) mutable {
            *mouseDeltaX = aEvent.GetXOffset();
            *mouseDeltaY = aEvent.GetYOffset();
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        float pitch{}, yaw{};
        auto update = [=, &entity](const soge::UpdateEvent& aEvent) mutable {
            {
                const float x = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::D)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::A));
                const float y = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::SpaceBar)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::LeftShift));
                const float z = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::W)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::S));
                const auto direction =
                    cameraTransform.Right() * x + cameraTransform.Up() * y + cameraTransform.Forward() * z;
                cameraTransform.m_position += direction * aEvent.GetDeltaTime();
            }

            if (*mouseDeltaX != 0.0f || *mouseDeltaY != 0.0f)
            {
                yaw += *mouseDeltaX * aEvent.GetDeltaTime();
                pitch += *mouseDeltaY * aEvent.GetDeltaTime();
                cameraTransform.m_rotation = glm::quat{glm::vec3{pitch, yaw, 0.0f}};

                *mouseDeltaX = 0.0f;
                *mouseDeltaY = 0.0f;
            }

            entity.UpdateMatrix(cameraProjection * cameraTransform.ViewMatrix() * transform.WorldMatrix());
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
