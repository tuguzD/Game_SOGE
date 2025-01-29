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
    }

    void Game::Load(AccessTag)
    {
        PushLayer(new MainGameLayer());

        auto& container = GetDependencyContainer();
        const auto eventModule = GetModule<soge::EventModule>();
        const auto inputModule = GetModule<soge::InputModule>();
        const auto windowModule = GetModule<soge::WindowModule>();
        const auto graphicsModule = GetModule<soge::GraphicsModule>();
        auto& entities = graphicsModule->GetEntityManager();

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

        auto board = soge::CreateShared<Board>();
        board->init(entities, container);

        auto cursorDark = soge::CreateShared<Cursor>(Cursor{.darkTeam = true});
        cursorDark->init(entities, container);

        auto cursorLight = soge::CreateShared<Cursor>(Cursor{.darkTeam = false});
        cursorLight->init(entities, container);

        // move cursor with arrow keys
        {
            cursorDark->color(entities, *board);
            cursorLight->color(entities, *board);

            auto cursorMove = [=, &entities](const soge::KeyPressedEvent& aEvent) mutable {
                glm::ivec2 cells{};
                soge::Key keys[8] = {
                    soge::Keys::W, soge::Keys::Up,
                    soge::Keys::S, soge::Keys::Down,
                    soge::Keys::A, soge::Keys::Left,
                    soge::Keys::D, soge::Keys::Right,
                };

                if (aEvent.GetKey() == keys[0] || aEvent.GetKey() == keys[1]) cells.x = +1;
                else if (aEvent.GetKey() == keys[2] || aEvent.GetKey() == keys[3]) cells.x = -1;
                else if (aEvent.GetKey() == keys[4] || aEvent.GetKey() == keys[5]) cells.y = -1;
                else if (aEvent.GetKey() == keys[6] || aEvent.GetKey() == keys[7]) cells.y = +1;

                auto cursor = *darkTeamMove ? cursorDark : cursorLight;
                cursor->move(entities, cells.x, cells.y);
                cursor->color(entities, *board);
            };
            eventModule->PushBack<soge::KeyPressedEvent>(cursorMove);
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
            entities.CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_APP_INFO_LOG(R"(Created directional light entity with UUID {})", lightUuid.str());
        const soge::Transform lightTransform{
            .m_rotation = glm::vec3{glm::radians(90.0f + 45.0f * (*darkTeamMove ? 1.0f : -1.0f)), 0.0f, 0.0f},
        };
        light.GetDirection() = lightTransform.Forward();

        auto cameraMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto cameraMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMove = [=](const soge::MouseMovedEvent& aEvent) {
            if (inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                *cameraMouseDeltaX = aEvent.GetXOffset();
                *cameraMouseDeltaY = aEvent.GetYOffset();
            }
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMove);

        {
            cursorDark->toggle(entities, cursorDark->darkTeam == *darkTeamMove);
            cursorLight->toggle(entities, cursorLight->darkTeam == *darkTeamMove);

            auto delayNextFrame = false;
            auto needTeamSwitch = soge::CreateShared<bool>(false);
            auto switchTeam = [=, &light, &entities](const soge::UpdateEvent&) mutable {
                // skip 1 second before switching team
                if (!*needTeamSwitch) return;
                if (!delayNextFrame)
                {
                    delayNextFrame = true;
                    return;
                }
                *needTeamSwitch = false;
                delayNextFrame = false;
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // actually switch team
                *darkTeamMove = !*darkTeamMove;
                SOGE_APP_INFO_LOG(R"(It's now "{}" team move!)", *darkTeamMove ? "dark" : "light");

                // switch active team cursor
                cursorDark->toggle(entities, cursorDark->darkTeam == *darkTeamMove);
                cursorLight->toggle(entities, cursorLight->darkTeam == *darkTeamMove);

                // switch team camera
                graphicsModule->GetViewportManager().GetViewport(viewportUuid)->m_cameraId =
                    *darkTeamMove ? cameraDarkUuid : cameraLightUuid;

                // switch scene light direction
                const auto angle = glm::radians(90.0f + 45.0f * (*darkTeamMove ? 1.0f : -1.0f));
                light.GetDirection() = soge::Transform{
                    .m_rotation = glm::vec3{angle, 0.0f, 0.0f}}.Forward();
            };
            eventModule->PushBack<soge::UpdateEvent>(switchTeam);

            // auto bruh = [=, &entities](glm::ivec2 a, glm::ivec2 direction, int modifier) {
            //     auto b = glm::ivec2{
            //         Board::clamp_cell(a.x + direction.x * modifier),
            //         Board::clamp_cell(a.y + direction.y * modifier),
            //     };
            //     if (a.x == b.x || a.y == b.y) return nullptr;
            //
            //     const auto entity = dynamic_cast<soge::StaticMeshEntity*>(
            //         entities.GetEntity(board->matrix[b.x][b.y].uuid));
            //     return (entity != nullptr);
            // };

            auto makeMove = [=, &entities](const soge::KeyPressedEvent& aEvent) mutable {
                soge::Key keys[2] = {soge::Keys::Q, soge::Keys::E};
                if (std::ranges::find(keys, aEvent.GetKey()) != std::end(keys))
                {
                    const auto cursor = dynamic_cast<soge::BoxPrimitive*>(
                        entities.GetEntity((*darkTeamMove ? cursorDark : cursorLight)->uuid));
                    if (cursor == nullptr)
                        return;

                    auto a = Board::get_cells(false, cursor->GetTransform());

                    const auto pieceAtCursor = dynamic_cast<soge::StaticMeshEntity*>(
                        entities.GetEntity(board->matrix[a.x][a.y].uuid));
                    if (pieceAtCursor == nullptr || board->matrix[a.x][a.y].darkTeam != *darkTeamMove)
                        return;

                    const auto modifier = *darkTeamMove ? -1 : 1;
                    const auto direction = aEvent.GetKey() == soge::Keys::Q ? -1 : 1;

                    auto forward = glm::ivec2{
                        Board::clamp_cell(a.x + modifier * direction),
                        Board::clamp_cell(a.y + modifier),
                    };
                    if (a.x == forward.x || a.y == forward.y) return;

                    const auto forwardPiece = dynamic_cast<soge::StaticMeshEntity*>(
                        entities.GetEntity(board->matrix[forward.x][forward.y].uuid));
                    if (forwardPiece == nullptr)
                    {
                        std::swap(board->matrix[a.x][a.y], board->matrix[forward.x][forward.y]);
                        
                        SOGE_APP_INFO_LOG(R"(Successfully move at ({}, {}))", forward.x, forward.y);
                        *needTeamSwitch = true;
                    }
                    else
                    {
                        if (board->matrix[forward.x][forward.y].darkTeam == *darkTeamMove)
                            return;

                        auto new_forward = glm::ivec2{
                            Board::clamp_cell(forward.x + modifier * direction),
                            Board::clamp_cell(forward.y + modifier),
                        };
                        if (forward.x == new_forward.x || forward.y == new_forward.y) return;

                        const auto newForwardPieceEntity = dynamic_cast<soge::StaticMeshEntity*>(
                            entities.GetEntity(board->matrix[new_forward.x][new_forward.y].uuid));
                        if (newForwardPieceEntity == nullptr)
                        {
                            std::swap(board->matrix[a.x][a.y], board->matrix[new_forward.x][new_forward.y]);
                            entities.DestroyEntity(board->matrix[forward.x][forward.y].uuid);
                            board->matrix[forward.x][forward.y] = Piece{};

                            SOGE_APP_INFO_LOG(R"(Successfully eat enemy at ({}, {}))", forward.x, forward.y);
                            *needTeamSwitch = true;
                        }
                        else
                        {
                            return;
                        }
                    }
                    board->sync(entities);
                    cursorDark->color(entities, *board);
                    cursorLight->color(entities, *board);
                }
            };
            eventModule->PushBack<soge::KeyPressedEvent>(makeMove);
        }

        // rotate camera with mouse drag
        {
            constexpr float cameraSensitivity = 0.0025f;
            float cameraLightPitch{glm::radians(50.0f)}, cameraLightYaw{};
            constexpr auto yawDarkOffset = 180.0f;
            float cameraDarkPitch{glm::radians(50.0f)}, cameraDarkYaw{glm::radians(yawDarkOffset)};

            auto cameraRotate = [=, &cameraLight, &cameraDark](const soge::UpdateEvent&) mutable {
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
            eventModule->PushBack<soge::UpdateEvent>(cameraRotate);
        }
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
