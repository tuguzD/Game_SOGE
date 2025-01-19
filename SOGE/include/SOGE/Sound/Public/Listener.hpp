#ifndef SOGE_SOUND_PUBLIC_LISTENER_HPP
#define SOGE_SOUND_PUBLIC_LISTENER_HPP

#include <glm/glm.hpp>


namespace soge
{
    class Listener final
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_forwardVector;
        glm::vec3 m_upwardVector;

    public:
        Listener();

        Listener(const glm::vec3& aPosition,
                 const glm::vec3& aForwardVec,
                 const glm::vec3& aUpwardVec);

        Listener(float aPosX,       float aPosY,        float aPosZ,
                 float aForwardX,   float aForwardY,    float aForwardZ,
                 float aUpwardX,    float aUpwardY,     float aUpwardZ);
        ~Listener() = default;

        void Update(const glm::vec3& aPosition,
                    const glm::vec3& aForwardVec,
                    const glm::vec3& aUpwardVec);

        void SetPosition(const glm::vec3& aPosition);
        void SetForwardVector(const glm::vec3& aForwardVec);
        void SetUpwardVector(const glm::vec3& aUpwardVec);

        const glm::vec3& GetPosition() const;
        const glm::vec3& GetForwardVector() const;
        const glm::vec3& GetUpwardVector() const;

    };
}

#endif // !SOGE_SOUND_PUBLIC_LISTENER_HPP
