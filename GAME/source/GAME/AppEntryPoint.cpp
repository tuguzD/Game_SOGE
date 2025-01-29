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

        auto darkTeamMove = soge::CreateShared<bool>(false);

        auto board = Board{};
        board.init(graphicsModule->GetEntityManager(), container);

        auto cursorDark = Cursor{.darkTeam = true};
        cursorDark.init(graphicsModule->GetEntityManager(), container);

        auto cursorLight = Cursor{.darkTeam = false};
        cursorLight.init(graphicsModule->GetEntityManager(), container);

        {
            cursorDark.color(graphicsModule->GetEntityManager(), board);
            cursorLight.color(graphicsModule->GetEntityManager(), board);

            auto cursorUpdate = [=](const soge::KeyPressedEvent& aEvent) mutable {
                glm::ivec2 cells{};
                soge::Key keys[8] = {
                    soge::Keys::W, soge::Keys::Up,
                    soge::Keys::S, soge::Keys::Down,
                    soge::Keys::D, soge::Keys::Right,
                    soge::Keys::A, soge::Keys::Left,
                };

                if (aEvent.GetKey() == keys[0] || aEvent.GetKey() == keys[1]) cells.x = 1;
                else if (aEvent.GetKey() == keys[2] || aEvent.GetKey() == keys[3]) cells.x = -1;
                else if (aEvent.GetKey() == keys[4] || aEvent.GetKey() == keys[5]) cells.y = 1;
                else if (aEvent.GetKey() == keys[6] || aEvent.GetKey() == keys[7]) cells.y = -1;

                auto cursor = *darkTeamMove ? cursorDark : cursorLight;
                cursor.move(graphicsModule->GetEntityManager(), cells.x, cells.y,
                    std::ranges::find(keys, aEvent.GetKey()) != std::end(keys));
                cursor.color(graphicsModule->GetEntityManager(), board);
            };
            eventModule->PushBack<soge::KeyPressedEvent>(cursorUpdate);
        }

        // setup viewport, cameras, and light
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

        const auto [viewport, viewportUuid] = graphicsModule->GetViewportManager().CreateViewport({
            .m_viewport = {static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight())},
            .m_cameraId = *darkTeamMove ? cameraDarkUuid : cameraLightUuid,
        });
        SOGE_APP_INFO_LOG(R"(Created viewport with UUID {})", viewportUuid.str());

        const auto [light, lightUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_APP_INFO_LOG(R"(Created directional light entity with UUID {})", lightUuid.str());
        const soge::Transform lightTransform{
            .m_rotation = glm::vec3{glm::radians(90.0f + 45.0f * (*darkTeamMove ? 1.0f : -1.0f)), 0.0f, 0.0f},
        };
        light.GetDirection() = lightTransform.Forward();

        auto cameraMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto cameraMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [=](const soge::MouseMovedEvent& aEvent) {
            if (inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                *cameraMouseDeltaX = aEvent.GetXOffset();
                *cameraMouseDeltaY = aEvent.GetYOffset();
            }
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        // switch teams with a key (for now)
        {
            cursorDark.toggle(graphicsModule->GetEntityManager(), cursorDark.darkTeam == *darkTeamMove);
            cursorLight.toggle(graphicsModule->GetEntityManager(), cursorLight.darkTeam == *darkTeamMove);

            auto teamMoveSwitch = [=, &light] (const soge::KeyPressedEvent& aEvent) mutable {
                if (aEvent.GetKey() == soge::Keys::Q)
                {
                    *darkTeamMove = !*darkTeamMove;
                    SOGE_APP_INFO_LOG(R"(It's now "{}" team move!)", *darkTeamMove ? "dark" : "light");

                    // switch active team cursor
                    cursorDark.toggle(graphicsModule->GetEntityManager(),
                        cursorDark.darkTeam == *darkTeamMove);
                    cursorLight.toggle(graphicsModule->GetEntityManager(),
                        cursorLight.darkTeam == *darkTeamMove);

                    // switch team camera
                    graphicsModule->GetViewportManager().GetViewport(viewportUuid)->m_cameraId =
                        *darkTeamMove ? cameraDarkUuid : cameraLightUuid;

                    // switch scene light direction
                    const auto angle = glm::radians(90.0f + 45.0f * (*darkTeamMove ? 1.0f : -1.0f));
                    light.GetDirection() = soge::Transform{
                        .m_rotation = glm::vec3{angle, 0.0f, 0.0f}}.Forward();
                }
            };
            eventModule->PushBack<soge::KeyPressedEvent>(teamMoveSwitch);
        }

        // rotate camera with mouse drag
        {
            constexpr float cameraSensitivity = 0.0025f;
            float cameraLightPitch{glm::radians(50.0f)}, cameraLightYaw{};
            constexpr auto yawDarkOffset = 180.0f;
            float cameraDarkPitch{glm::radians(50.0f)}, cameraDarkYaw{glm::radians(yawDarkOffset)};

            auto cameraUpdate = [=, &cameraLight, &cameraDark](const soge::UpdateEvent&) mutable {
                if (*darkTeamMove)
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
