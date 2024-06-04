#include "math.hh"

using namespace Game;

vec2DF Math::unitVector2d(float x, float y)
{
    float len = std::sqrt(x*x + y*y);
    return (len == 0.0f) ? vec2DF{0.0f, 0.0f} : vec2DF{x/len, y/len};
}

vec2DF Math::unitVector2d(int x, int y)
{
    return unitVector2d((float)x, (float)y);
}

float Math::norm(const vec2DF &vec)
{
    return std::sqrt(vec.x*vec.x + vec.y*vec.y);
}
