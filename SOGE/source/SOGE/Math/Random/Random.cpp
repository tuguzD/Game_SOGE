#include "sogepch.hpp"
#include "SOGE/Math/Random/Random.hpp"


namespace soge
{
    Random::Random() : m_randEngine128()
    {
    }

    Random::Random(unsigned int aSeed)
    {
        Seed(aSeed);
    }

    Random::~Random()
    {
    }

    void Random::Seed(unsigned int aSeed)
    {
        m_randEngine128 = XoshiroCpp::Xoshiro128PlusPlus(aSeed);
    }

    float Random::GenNormalizedFloat() const
    {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(m_randEngine128);
    }

    float Random::GenFloat(float aMin, float aMax) const
    {
        std::uniform_real_distribution<float> dist(aMin, aMax);
        return dist(m_randEngine128);
    }

    int Random::GenInt(int aMin, int aMax) const
    {
        std::uniform_int_distribution dist(aMin, aMax);
        return dist(m_randEngine128);
    }

    bool Random::GenBool() const
    {
        std::uniform_int_distribution dist(0, 1);
        return static_cast<bool>(dist(m_randEngine128));
    }

    glm::vec2 Random::GenVec2(const glm::vec2& aMin, const glm::vec2& aMax) const
    {
        return glm::linearRand(aMin, aMax);
    }

    glm::vec3 Random::GenVec3(const glm::vec3& aMin, const glm::vec3& aMax) const
    {
        return glm::linearRand(aMin, aMax);
    }

    glm::vec4 Random::GenVec4(const glm::vec4& aMin, const glm::vec4& aMax) const
    {
        return glm::linearRand(aMin, aMax);
    }

    float Random::GenGaussianFloat(float aMean, float aDeviation) const
    {
        return glm::gaussRand(aMean, aDeviation);
    }
}
