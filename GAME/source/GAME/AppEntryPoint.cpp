#include "GAME/AppEntryPoint.hpp"

#include "GAME/Cursor.hpp"
#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/Deferred/DeferredRenderGraph.hpp>
#include <SOGE/Graphics/Entities/DirectionalLightEntity.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Math/Camera.hpp>
#include <SOGE/Window/WindowModule.hpp>
#include <algorithm>

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
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Game::Load(AccessTag)
    {
        PushLayer(new MainGameLayer());

        auto& container = GetDependencyContainer();
        const auto eventModule = GetModule<soge::EventModule>();
        const auto inputModule = GetModule<soge::InputModule>();
        const auto windowModule = GetModule<soge::WindowModule>();
        const auto graphicsModule = GetModule<soge::GraphicsModule>();

        soge::Logger::GetEngineSideLogger()->set_level(soge::Logger::Level::warn);

        const auto [window, windowUuid] = windowModule->CreateWindow(
            soge::WindowDesc{L"Game", 1920u, 1080u});
        SOGE_APP_INFO_LOG(
            R"(Created window "{}" of width {} and height {} with UUID {})",
            soge::EAToNarrow(window.GetTitle()).c_str(),
            window.GetWidth(), window.GetHeight(), windowUuid.str());
        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::DeferredRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        auto board = Board{};
        board.init(graphicsModule->GetEntityManager(), container);

        // setup cursor for team of light pieces
        {
            auto cursorLight = Cursor{.darkTeam = false};
            cursorLight.init(graphicsModule->GetEntityManager(), container);
            cursorLight.color(graphicsModule->GetEntityManager(), board);

            auto cursorLightUpdate = [graphicsModule, cursorLight, board](const soge::KeyPressedEvent& aEvent) mutable {
                glm::ivec2 cells{};
                soge::Key keys[4] = {soge::Keys::Up, soge::Keys::Down,
                    soge::Keys::Right, soge::Keys::Left};

                if (aEvent.GetKey() == keys[0]) cells.x = 1;
                else if (aEvent.GetKey() == keys[1]) cells.x = -1;
                else if (aEvent.GetKey() == keys[2]) cells.y = 1;
                else if (aEvent.GetKey() == keys[3]) cells.y = -1;

                cursorLight.move(graphicsModule->GetEntityManager(), cells.x, cells.y,
                    std::ranges::find(keys, aEvent.GetKey()) != std::end(keys));
                cursorLight.color(graphicsModule->GetEntityManager(), board);
            };
            eventModule->PushBack<soge::KeyPressedEvent>(cursorLightUpdate);
        }

        // setup cursor for team of dark pieces
        {
            auto cursorDark = Cursor{.darkTeam = true};
            cursorDark.init(graphicsModule->GetEntityManager(), container);
            cursorDark.color(graphicsModule->GetEntityManager(), board);

            auto cursorDarkUpdate = [graphicsModule, cursorDark, board](const soge::KeyPressedEvent& aEvent) mutable {
                glm::ivec2 cells{};
                soge::Key keys[4] = {soge::Keys::W, soge::Keys::S, soge::Keys::D, soge::Keys::A};

                if (aEvent.GetKey() == keys[0]) cells.x = 1;
                else if (aEvent.GetKey() == keys[1]) cells.x = -1;
                else if (aEvent.GetKey() == keys[2]) cells.y = 1;
                else if (aEvent.GetKey() == keys[3]) cells.y = -1;

                cursorDark.move(graphicsModule->GetEntityManager(), cells.x, cells.y,
                    std::ranges::find(keys, aEvent.GetKey()) != std::end(keys));
                cursorDark.color(graphicsModule->GetEntityManager(), board);
            };
            eventModule->PushBack<soge::KeyPressedEvent>(cursorDarkUpdate);
        }

        // example stuff

        const auto [light, lightUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_APP_INFO_LOG(R"(Created directional light entity with UUID {})", lightUuid.str());
        const soge::Transform lightTransform{
            .m_rotation = glm::vec3{glm::radians(135.0f - 90.0f), 0.0f, 0.0f},
        };
        light.GetDirection() = lightTransform.Forward();

        const auto [cameraLight, cameraLightUuid] = graphicsModule->GetCameraManager().CreateCamera({
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f, .m_farPlane = 100.0f,
            .m_transform = soge::Transform{
                .m_position = glm::vec3{0.0f, 3.5f, -4.0f},
                .m_rotation = glm::vec3{glm::radians(50.0f), 0.0f, 0.0f},
            },
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });
        SOGE_APP_INFO_LOG(R"(Created "light" camera with UUID {})", cameraLightUuid.str());

        const auto [cameraDark, cameraDarkUuid] = graphicsModule->GetCameraManager().CreateCamera({
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f, .m_farPlane = 100.0f,
            .m_transform = soge::Transform{
                .m_position = glm::vec3{0.0f, 3.5f, 4.0f},
                .m_rotation = glm::vec3{glm::radians(50.0f), glm::radians(180.0f), 0.0f},
            },
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });
        SOGE_APP_INFO_LOG(R"(Created "dark" camera with UUID {})", cameraDarkUuid.str());

        auto darkCameraActive = soge::CreateShared<bool>(false);
        const auto [viewport, viewportUuid] = graphicsModule->GetViewportManager().CreateViewport({
            .m_viewport = {static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight())},
            .m_cameraId = *darkCameraActive ? cameraDarkUuid : cameraLightUuid,
        });
        SOGE_APP_INFO_LOG(R"(Created viewport with UUID {})", viewportUuid.str());

        auto cameraMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto cameraMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [cameraMouseDeltaX, cameraMouseDeltaY, inputModule](const soge::MouseMovedEvent& aEvent) {
            if (inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                *cameraMouseDeltaX = aEvent.GetXOffset();
                *cameraMouseDeltaY = aEvent.GetYOffset();
            }
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        // switch cameras
        {
            auto cameraSwitch = [cameraDarkUuid, cameraLightUuid, graphicsModule, viewportUuid, darkCameraActive, &light]
            (const soge::KeyPressedEvent& aEvent) mutable {
                if (aEvent.GetKey() == soge::Keys::Q)
                {
                    *darkCameraActive = !*darkCameraActive;
                    const auto angle = glm::radians(90.0f + 45.0f * (*darkCameraActive ? 1.0f : -1.0f));
                    
                    SOGE_APP_INFO_LOG(R"(Switched to "{}" camera)", *darkCameraActive ? "dark" : "light");
                    graphicsModule->GetViewportManager().GetViewport(viewportUuid)->m_cameraId =
                        *darkCameraActive ? cameraDarkUuid : cameraLightUuid;
                    light.GetDirection() = soge::Transform{
                        .m_rotation = glm::vec3{angle, 0.0f, 0.0f}}.Forward();
                }
            };
            eventModule->PushBack<soge::KeyPressedEvent>(cameraSwitch);
        }

        // rotate camera with mouse drag
        {
            constexpr float cameraSensitivity = 0.0025f;
            float cameraLightPitch{glm::radians(50.0f)}, cameraLightYaw{};
            constexpr auto yawDarkOffset = 180.0f;
            float cameraDarkPitch{glm::radians(50.0f)}, cameraDarkYaw{glm::radians(yawDarkOffset)};

            auto cameraUpdate = [=, &cameraLight, &cameraDark](const soge::UpdateEvent&) mutable {
                if (*darkCameraActive)
                {
                    cameraDarkYaw += *cameraMouseDeltaX * cameraSensitivity;
                    cameraDarkPitch += *cameraMouseDeltaY * cameraSensitivity;
                    cameraDark.m_transform.m_rotation = glm::vec3{
                        glm::clamp(cameraDarkPitch, glm::radians(40.0f), glm::radians(60.0f)),
                        glm::clamp(cameraDarkYaw, glm::radians(yawDarkOffset - 30),
                            glm::radians(yawDarkOffset + 30)), 0.0f,
                    };
                }
                else
                {
                    cameraLightYaw += *cameraMouseDeltaX * cameraSensitivity;
                    cameraLightPitch += *cameraMouseDeltaY * cameraSensitivity;
                    cameraLight.m_transform.m_rotation = glm::vec3{
                        glm::clamp(cameraLightPitch, glm::radians(40.0f), glm::radians(60.0f)),
                        glm::clamp(cameraLightYaw, glm::radians(-30.0f),
                            glm::radians(30.0f)), 0.0f,
                    };
                }
                *cameraMouseDeltaX = 0.0f;
                *cameraMouseDeltaY = 0.0f;
            };
            eventModule->PushBack<soge::UpdateEvent>(cameraUpdate);
        }
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
