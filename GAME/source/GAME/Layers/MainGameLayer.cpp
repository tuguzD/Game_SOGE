#include "GAME/Layers/MainGameLayer.hpp"


namespace soge_game
{
    MainGameLayer::MainGameLayer() : soge::Layer("MainGameLayer")
    {
    }

    MainGameLayer::~MainGameLayer()
    {
    }

    void MainGameLayer::OnAttach()
    {
        SOGE_APP_INFO_LOG("Layer {0} attached", this->m_layerName.c_str());
    }

    void MainGameLayer::OnDetach()
    {
    }

    void MainGameLayer::OnUpdate()
    {
        auto inputModule = soge::Engine::GetInstance()->GetModule<soge::InputModule>();
        if (inputModule->IsKeyPressed(soge::Keys::W))
        {
            SOGE_APP_INFO_LOG("Key W pressed");
        }
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
