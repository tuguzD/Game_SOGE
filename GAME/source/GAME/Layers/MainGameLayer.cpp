#include "GAME/Layers/MainGameLayer.hpp"
#include "GAME/AppEntryPoint.hpp"


namespace soge_game
{
    MainGameLayer::MainGameLayer() : soge::Layer("MainGameLayer")
    {
        SOGE_APP_INFO_LOG("Test audio path: {}", test.GetFullPath().c_str());
        SOGE_APP_INFO_LOG("Test audio resource name: {}", test.GetName().c_str());
        SOGE_APP_INFO_LOG("Test audio resource uuid: {}", test.GetUUID().str());
        test.SetVolume(100);
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

        auto soundModule = Game::GetInstance()->GetModule<soge::SoundModule>();
        soundModule->LoadSoundResource(test);
        soundModule->PlaySoundResource(test);
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
