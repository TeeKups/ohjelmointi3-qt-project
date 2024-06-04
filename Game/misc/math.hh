#ifndef MATH_HH
#define MATH_HH

#include <cmath>
#include <algorithm>
#include <QDebug>
#include "types.hh"

/**
 * @file
 * @brief Defines a Math class
 */

namespace Game {

    /**
     * @brief The Math class contains some math functions
     */
    class Math
    {
    public:
        /**
         * @brief Calculates a unit vector defined by parameters
         * @param x: Real part
         * @param y: Im part
         * @return [x y]' / sqrt(x^2 + y^2)
         */
        static vec2DF unitVector2d(float x, float y);

        /**
         * @brief Calculates a unit vector defined by parameters
         * @param x: Real part
         * @param y: Im part
         * @return [x y]' / sqrt(x^2 + y^2)
         */
        static vec2DF unitVector2d(int x, int y);

        /**
         * @brief Euclidean norm of 2D-vector
         * @param vec: vector whose norm is calculated
         * @return sqrt(vec.x^2 + vec.y^2)
         */
        static float norm(const vec2DF& vec);
    };

}

#endif // MATH_HH
