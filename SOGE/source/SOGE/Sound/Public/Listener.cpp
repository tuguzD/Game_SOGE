#include "sogepch.hpp"
#include "SOGE/Sound/Public/Listener.hpp"


namespace soge
{
    Listener::Listener()
    {
        SetPosition({0.0f, 0.0f, -1.0f});
        SetForwardVector({0.0f, 0.0f, 1.0f});
        SetUpwardVector({0.0f, 1.0f, 0.0f});
    }

    Listener::Listener(const glm::vec3& aPosition,
                       const glm::vec3& aForwardVec,
                       const glm::vec3& aUpwardVec)
    {
        SetPosition(aPosition);
        SetForwardVector(aForwardVec);
        SetUpwardVector(aUpwardVec);
    }

    Listener::Listener(float aPosX,     float aPosY,        float aPosZ,
                       float aForwardX, float aForwardY,    float aForwardZ,
                       float aUpwardX,  float aUpwardY,     float aUpwardZ)
    {
        SetPosition({aPosX, aPosY, aPosZ});
        SetForwardVector({aForwardX, aForwardY, aForwardZ});
        SetUpwardVector({aUpwardX, aUpwardY, aUpwardZ});
    }

    void Listener::Update(const glm::vec3& aPosition,
                          const glm::vec3& aForwardVec,
                          const glm::vec3& aUpwardVec)
    {
        SetPosition(aPosition);
        SetForwardVector(aForwardVec);
        SetUpwardVector(aUpwardVec);
    }

    void Listener::SetPosition(const glm::vec3& aPosition)
    {
        m_position = aPosition;
    }

    void Listener::SetForwardVector(const glm::vec3& aForwardVec)
    {
        m_forwardVector = aForwardVec;
    }

    void Listener::SetUpwardVector(const glm::vec3& aUpwardVec)
    {
        m_upwardVector = aUpwardVec;
    }

    const glm::vec3& Listener::GetPosition() const
    {
        return m_position;
    }

    const glm::vec3& Listener::GetForwardVector() const
    {
        return m_forwardVector;
    }

    const glm::vec3& Listener::GetUpwardVector() const
    {
        return m_upwardVector;
    }
}
