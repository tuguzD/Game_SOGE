#ifndef SOGE_MATH_RANDOM_RANDOM_HPP
#define SOGE_MATH_RANDOM_RANDOM_HPP

#include <XoshiroCpp/XoshiroCpp.hpp>
#include <glm/gtc/random.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace soge
{
    /**
     * @class Random Random.hpp "SOGE/Math/Random.hpp"
     * @brief A utility class to generate random values.
     */
    class Random
    {
    private:
        mutable XoshiroCpp::Xoshiro128PlusPlus m_randEngine128;

    public:
        Random();
        Random(unsigned int aSeed);
        ~Random();

        /**
        * @brief Initialize the random number generator with a seed.
        * @param[in] aSeed - randomization seed.
        */
        void Seed(unsigned int aSeed);

        /**
         * @brief Generates a random float value between [0.0f, 1.0f].
         * @returns float number in range [0.0f, 1.0f].
         */
        float GenNormalizedFloat() const;

        /**
         * @brief Generates a random float value between [aMin, aMax].
         * @returns float number in range [aMin, aMax].
         * @param[in] aMin - minimal float value.
         * @param[in] aMax - maximal float value.
         */
        float GenFloat(float aMin, float aMax) const;

        /**
         * @brief Generates a random int value between [aMin, aMax]
         * @returns real number in range [aMin, aMax]
         * @param[in] aMin - minimal int value.
         * @param[in] aMax - maximal int value.
         */
        int GenInt(int aMin, int aMax) const;

        /**
         * @brief Generates a random bool value.
         * @returns random true or false value.
         */
        bool GenBool() const;

        /**
         * @brief Generates a random vector2 value between [VectorMin, VectorMax]
         * @returns glm::vec2 in range [aMin, aMax]
         * @param[in] aMin - minimal vector value.
         * @param[in] aMax - maximal vector value.
         */
        glm::vec2 GenVec2(const glm::vec2& aMin, const glm::vec2& aMax) const;

        /**
         * @brief Generates a random vector3 value between [VectorMin, VectorMax]
         * @returns glm::vec3 in range [aMin, aMax]
         * @param[in] aMin - minimal vector value.
         * @param[in] aMax - maximal vector value.
         */
        glm::vec3 GenVec3(const glm::vec3& aMin, const glm::vec3& aMax) const;

        /**
         * @brief Generates a random vector4 value between [VectorMin, VectorMax]
         * @returns glm::vec4 in range [aMin, aMax]
         * @param[in] aMin - minimal vector value.
         * @param[in] aMax - maximal vector value.
         */
        glm::vec4 GenVec4(const glm::vec4& aMin, const glm::vec4& aMax) const;

        /**
         * @brief Generates gaussian normal distribution value.
         * @returns Gaussian normal distribution value.
         * @param[in] aMean - mean of the distribution (central value). Most of
         * the generated random numbers will cluster around this mean.
         * @param[in] aDeviation - deviation of the distribution. The
         * actual spread (width) of the distribution.
         */
        float GenGaussianFloat(float aMean, float aDeviation) const;
    };
}

#endif // SOGE_MATH_RANDOM_RANDOM_HPP
