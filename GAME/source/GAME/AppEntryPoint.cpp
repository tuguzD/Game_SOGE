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
#include <SOGE/Math/Camera.hpp>
#include <SOGE/Window/WindowModule.hpp>

#undef CreateWindow


namespace
{
    using Vertex = soge::GeometryEntity::Vertex;

    constexpr std::array<Vertex, 24> BoxVertices(const float aLength = 1.0f, const float aHeight = 1.0f,
                                                 const float aWidth = 1.0f, const glm::vec4& aColor = glm::vec4{1.0f})
    {
        constexpr glm::vec4 red{0.91f, 0.36f, 0.36f, 1.0f};
        constexpr glm::vec4 green{0.37f, 0.82f, 0.47f, 1.0f};
        constexpr glm::vec4 blue{0.42f, 0.49f, 0.95f, 1.0f};
        constexpr glm::vec4 gray{0.28f, 0.28f, 0.28f, 1.0f};

        const float right = aLength / 2;
        const float left = -aLength / 2;
        const float top = aHeight / 2;
        const float bottom = -aHeight / 2;
        const float forward = aWidth / 2;
        const float backward = -aWidth / 2;

        return {
            // Front face
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor * blue /*glm::vec2{1.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor * green /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor * red /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor * gray /*, glm::vec2{1.0f, 0.0f}*/,
            },

            // Back face
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor * red /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor * green /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor * blue /*, glm::vec2{1.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor * gray /*, glm::vec2{1.0f, 1.0f}*/,
            },

            // Top Face
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor * green /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor * gray /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor * red /*, glm::vec2{1.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor * blue /*, glm::vec2{1.0f, 1.0f}*/,
            },

            // Bottom Face
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor * red /*, glm::vec2{1.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor * gray /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor * green /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor * blue /*, glm::vec2{1.0f, 0.0f}*/,
            },

            // Left Face
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor * blue /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor * gray /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor * green /*, glm::vec2{1.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor * red /*, glm::vec2{1.0f, 1.0f}*/,
            },

            // Right Face
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor * gray /*, glm::vec2{0.0f, 1.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor * blue /*, glm::vec2{0.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor * red /*, glm::vec2{1.0f, 0.0f}*/,
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor * green /*, glm::vec2{1.0f, 1.0f}*/,
            },
        };
    }

    using Index = soge::GeometryEntity::Index;

    constexpr std::array<Index, 36> BoxIndices()
    {
        // clang-format off
        return {
            // Front face
            0u, 1u, 2u,
            0u, 2u, 3u,
            // Back face
            4u, 5u, 6u,
            4u, 6u, 7u,
            // Top Face
            8u, 9u, 10u,
            8u, 10u, 11u,
            // Bottom Face
            12u, 13u, 14u,
            12u, 14u, 15u,
            // Left Face
            16u, 17u, 18u,
            16u, 18u, 19u,
            // Right Face
            20u, 21u, 22u,
            20u, 22u, 23u,
        };
        // clang-format on
    }

    // https://github.com/caosdoar/spheres/blob/master/src/spheres.cpp#L262
    std::pair<std::vector<Vertex>, std::vector<Index>> UvSphere(const uint32_t aMeridians = 32,
                                                                const uint32_t aParallels = 16,
                                                                const float aRadius = 1.0f)
    {
        std::vector<Vertex> vertices;
        vertices.emplace_back(glm::vec3{0.0f, 1.0f, 0.0f} * aRadius, glm::vec3{0.0f, 1.0f, 0.0f});
        for (uint32_t j = 0; j < aParallels - 1; ++j)
        {
            double const polar = glm::pi<float>() * static_cast<double>(j + 1) / static_cast<double>(aParallels);
            double const sp = std::sin(polar);
            double const cp = std::cos(polar);
            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                double const azimuth =
                    2.0 * glm::pi<float>() * static_cast<double>(i) / static_cast<double>(aMeridians);
                double const sa = std::sin(azimuth);
                double const ca = std::cos(azimuth);
                double const x = sp * ca;
                double const y = cp;
                double const z = sp * sa;
                vertices.emplace_back(glm::vec3{x, y, z} * aRadius, glm::vec3{x, y, z});
            }
        }
        vertices.emplace_back(glm::vec3{0.0f, -1.0f, 0.0f} * aRadius, glm::vec3{0.0f, -1.0f, 0.0f});

        std::vector<Index> indices;
        for (uint32_t i = 0; i < aMeridians; ++i)
        {
            uint32_t const a = i + 1;
            uint32_t const b = (i + 1) % aMeridians + 1;

            indices.push_back(0);
            indices.push_back(b);
            indices.push_back(a);
        }
        for (uint32_t j = 0; j < aParallels - 2; ++j)
        {
            const uint32_t aStart = j * aMeridians + 1;
            const uint32_t bStart = (j + 1) * aMeridians + 1;
            for (uint32_t i = 0; i < aMeridians; ++i)
            {
                const uint32_t a = aStart + i;
                const uint32_t a1 = aStart + (i + 1) % aMeridians;
                const uint32_t b = bStart + i;
                const uint32_t b1 = bStart + (i + 1) % aMeridians;

                indices.push_back(a);
                indices.push_back(a1);
                indices.push_back(b1);

                indices.push_back(a);
                indices.push_back(b1);
                indices.push_back(b);
            }
        }
        for (uint32_t i = 0; i < aMeridians; ++i)
        {
            const uint32_t a = i + aMeridians * (aParallels - 2) + 1;
            const uint32_t b = (i + 1) % aMeridians + aMeridians * (aParallels - 2) + 1;

            indices.push_back(static_cast<uint32_t>(vertices.size()) - 1);
            indices.push_back(a);
            indices.push_back(b);
        }

        return {vertices, indices};
    }
}

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
                    const auto [entity, entityUuid] =
                        graphicsModule->GetEntityManager().CreateEntity<soge::GeometryEntity>(
                            container.Provide<soge::GeometryEntity>());
                    SOGE_INFO_LOG(R"(Created triangle entity ({}, {}, {}) with UUID {})", i, j, k, entityUuid.str());

                    constexpr std::array vertices = BoxVertices();
                    entity.UpdateVertices(vertices);

                    constexpr std::array indices = BoxIndices();
                    entity.UpdateIndices(indices);

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

        const auto [entity, entityUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::GeometryEntity>(
            container.Provide<soge::GeometryEntity>());
        SOGE_INFO_LOG(R"(Created triangle entity with UUID {})", entityUuid.str());

        const auto [vertices, indices] = UvSphere();
        entity.UpdateVertices(vertices);
        entity.UpdateIndices(indices);
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
