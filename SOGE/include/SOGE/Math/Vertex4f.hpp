#ifndef SOGE_MATH_VERTEX4F_HPP
#define SOGE_MATH_VERTEX4F_HPP


namespace soge
{
    // TODO: Remove after linking math library

    struct Vector4f
    {
    public:
        float x;
        float y;
        float z;
        float w;

    public:
        Vector4f(
            float aX = 0.0f,
            float aY = 0.0f,
            float aZ = 0.0f,
            float aW = 0.0f
        ) : x(aX), y(aY), z(aZ), w(aW)
        {

        }


    };
}

#endif // !SOGE_MATH_VERTEX4F_HPP
