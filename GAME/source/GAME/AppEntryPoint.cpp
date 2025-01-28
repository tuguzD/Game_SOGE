#include "GAME/AppEntryPoint.hpp"

#include "GAME/Piece.hpp"
#include "GAME/Layers/MainGameLayer.hpp"
#include "SOGE/Graphics/Primitives/Box.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/Deferred/DeferredRenderGraph.hpp>
#include <SOGE/Graphics/Entities/DirectionalLightEntity.hpp>
#include <SOGE/Graphics/Entities/StaticMeshEntity.hpp>
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

        soge::Logger::GetEngineSideLogger()->set_level(soge::Logger::Level::warn);

        const auto [window, windowUuid] = windowModule->CreateWindow();
        SOGE_APP_INFO_LOG(
            R"(Created window "{}" of width {} and height {} with UUID {})",
            soge::EAToNarrow(window.GetTitle()).c_str(),
            window.GetWidth(), window.GetHeight(), windowUuid.str()
        );

        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::DeferredRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        const auto [board, boardUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
                container.Provide<soge::StaticMeshEntity>());
        SOGE_APP_INFO_LOG(R"(Created board with UUID {})", boardUuid.str());
        board.GetFilePath() = "./resources/meshes/board/board.fbx";
        board.GetTransform() = soge::Transform{
            .m_rotation = glm::quat{glm::vec3{0.0f, glm::radians(90.0f), 0.0f}},
            .m_scale = glm::vec3{0.1f},
        };
        board.Load();

        constexpr std::int8_t matrix_order = 8;
        constexpr std::float_t piece_height = 0.26715f * 2;

        Piece board_matrix[matrix_order][matrix_order]{};

        const auto get_coords = [](const bool darkTeam, int cell) {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);
            cell = std::max(0, std::min(cell, matrix_order - 1));
            return offset * (1 - static_cast<float_t>(cell) * 2 / (matrix_order - 1));
        };
        const auto get_cell = [](const bool darkTeam, float_t coords) {
            const auto offset = 2.38f * static_cast<float_t>(!darkTeam ? -1 : 1);
            const auto result = (1 - coords / offset) * (matrix_order - 1) / 2;
            return static_cast<int>(round(result));
        };
        const auto clamp_cell = [](int cell) {
            return std::max(0, std::min(cell, matrix_order - 1));
        };

        for (std::size_t i = 0; i < 2; ++i)
        {
            const auto name = !i ? "light_piece" : "dark_piece";
            for (std::size_t j = 0; j < 3; ++j)
                for (std::size_t k = 0; k < 4; ++k)
                {
                    const auto [piece, pieceUuid] =
                        graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
                            container.Provide<soge::StaticMeshEntity>());
                    SOGE_APP_INFO_LOG(R"(Created {} with UUID {})", name, pieceUuid.str());
                    piece.GetFilePath() = std::format("./resources/meshes/{}/{}.fbx", name, name);

                    const auto z = static_cast<int>(j);
                    const auto x = z % 2 + static_cast<int>(k) * 2;
                    const auto coords_x = get_coords(i, x);
                    const auto coords_z = get_coords(i, z);

                    piece.GetTransform() = soge::Transform{
                        .m_position = glm::vec3{coords_x, piece_height, coords_z},
                        .m_scale = glm::vec3{0.1f},
                    };
                    piece.Load();

                    const auto uni_x = get_cell(false, coords_x);
                    const auto uni_z = get_cell(false, coords_z);
                    board_matrix[uni_x][uni_z] = Piece{
                        .uuid = pieceUuid,
                        .darkTeam = static_cast<bool>(i),
                    };
                }
        }

        const auto [cursor, cursorUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::BoxPrimitive>(
                container.Provide<soge::BoxPrimitive>());
        SOGE_APP_INFO_LOG(R"(Created box with UUID {})", cursorUuid.str());
        cursor.GetTransform() = soge::Transform{
            .m_position = glm::vec3{
                get_coords(false, 7), piece_height / 3, get_coords(false, 1),
            },
            .m_scale = glm::vec3{0.725f, 0.1f, 0.725f},
        };

        auto cursorUpdate = [=, &cursor](const soge::KeyPressedEvent& aEvent) mutable {
            auto cell_x = get_cell(false, cursor.GetTransform().m_position.x);
            auto cell_z = get_cell(false, cursor.GetTransform().m_position.z);

            if (aEvent.GetKey() == soge::Keys::Up)
            {
                cell_z = clamp_cell(cell_z + 1);
                cursor.GetTransform().m_position.z = get_coords(false, cell_z);
                SOGE_APP_INFO_LOG(R"(Current cursor location: ({}, {}))", cell_x, cell_z);
            }
            else if (aEvent.GetKey() == soge::Keys::Down)
            {
                cell_z = clamp_cell(cell_z - 1);
                cursor.GetTransform().m_position.z = get_coords(false, cell_z);
                SOGE_APP_INFO_LOG(R"(Current cursor location: ({}, {}))", cell_x, cell_z);
            }
            else if (aEvent.GetKey() == soge::Keys::Right)
            {
                cell_x = clamp_cell(cell_x + 1);
                cursor.GetTransform().m_position.x = get_coords(false, cell_x);
                SOGE_APP_INFO_LOG(R"(Current cursor location: ({}, {}))", cell_x, cell_z);
            }
            else if (aEvent.GetKey() == soge::Keys::Left)
            {
                cell_x = clamp_cell(cell_x - 1);
                cursor.GetTransform().m_position.x = get_coords(false, cell_x);
                SOGE_APP_INFO_LOG(R"(Current cursor location: ({}, {}))", cell_x, cell_z);
            }

            if (graphicsModule->GetEntityManager().GetEntity(board_matrix[cell_x][cell_z].uuid))
            {
                if (!board_matrix[cell_x][cell_z].darkTeam)
                    cursor.GetMaterial().m_diffuse = glm::vec3{0.0f, 0.75f, 0.0f};
                else
                    cursor.GetMaterial().m_diffuse = glm::vec3{0.75f, 0.0f, 0.0f};
            }
            else cursor.GetMaterial().m_diffuse = glm::vec3{1.0f, 1.0f, 1.0f};
        };
        eventModule->PushBack<soge::KeyPressedEvent>(cursorUpdate);

        const auto [directionalLightEntity1, directionalLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_APP_INFO_LOG(R"(Created directional light entity with UUID {})", directionalLightEntityUuid1.str());
        const soge::Transform directionalLightTransform1{
            .m_rotation = glm::vec3{glm::radians(45.0f), glm::radians(45.0f), 0.0f},
        };
        directionalLightEntity1.GetDirection() = directionalLightTransform1.Forward();

        const auto [camera, cameraUuid] = graphicsModule->GetCameraManager().CreateCamera({
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f,
            .m_farPlane = 100.0f,
            .m_transform = soge::Transform{.m_position = glm::vec3{0.0f, 0.0f, -2.0f}},
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });
        SOGE_APP_INFO_LOG(R"(Created camera with UUID {})", cameraUuid.str());

        const auto [viewport, viewportUuid] = graphicsModule->GetViewportManager().CreateViewport({
            .m_viewport = {static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight())},
            .m_cameraId = cameraUuid,
        });
        SOGE_APP_INFO_LOG(R"(Created viewport with UUID {})", viewportUuid.str());

        // share state between two lambdas
        auto cameraMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto cameraMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto lightMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto lightMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [cameraMouseDeltaX, cameraMouseDeltaY, lightMouseDeltaX, lightMouseDeltaY, inputModule]
        (const soge::MouseMovedEvent& aEvent) {
            if (inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                *cameraMouseDeltaX = aEvent.GetXOffset();
                *cameraMouseDeltaY = aEvent.GetYOffset();
            }
            if (inputModule->IsKeyPressed(soge::Keys::RightMouseButton))
            {
                *lightMouseDeltaX = aEvent.GetXOffset();
                *lightMouseDeltaY = aEvent.GetYOffset();
            }
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        float cameraPitch{}, cameraYaw{};
        float lightPitch{glm::radians(45.0f)}, lightYaw{glm::radians(45.0f)};
        constexpr float cameraSpeed = 1.0f;
        constexpr float cameraSensitivity = 0.005f;
        auto update = [=, &camera, &directionalLightEntity1](const soge::UpdateEvent& aEvent) mutable {
            {
                const float x = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::D)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::A));
                const float y = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::SpaceBar)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::LeftShift));
                const float z = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::W)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::S));
                const auto direction =
                    camera.m_transform.Right() * x + camera.m_transform.Up() * y + camera.m_transform.Forward() * z;
                camera.m_transform.m_position += direction * cameraSpeed * aEvent.GetDeltaTime();
            }

            cameraYaw += *cameraMouseDeltaX * cameraSensitivity;
            cameraPitch += *cameraMouseDeltaY * cameraSensitivity;
            camera.m_transform.m_rotation = glm::vec3{cameraPitch, cameraYaw, 0.0f};

            lightYaw += *lightMouseDeltaX * cameraSensitivity;
            lightPitch += *lightMouseDeltaY * cameraSensitivity;
            directionalLightEntity1.GetDirection() =
                soge::Transform{.m_rotation = glm::vec3{lightPitch, lightYaw, 0.0f}}.Forward();

            *cameraMouseDeltaX = 0.0f;
            *cameraMouseDeltaY = 0.0f;
            *lightMouseDeltaX = 0.0f;
            *lightMouseDeltaY = 0.0f;
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
