#ifndef TYPES_HH
#define TYPES_HH

#include <cmath>

/**
 * @file
 * @brief Defines some types for use in game
 */

namespace Game {

/**
 * enough for 4.3e9 unique ID's.
 * long unsigned int would be enough for 18.4e18 unique ID's. That's probably enough.
 */
using ID = unsigned int;

    static ID ID_REGISTER = 0; /** Incrementing counter for geriving more unique IDs */

    /**
     * @brief increments ID_REGISTER by one and returns the value;
     * @return A new unique ID
     */
    inline ID getNextID() { return ID_REGISTER++; };

    /**
     * @brief The ItemType enum is used to distinguish different types of actors
     */
    enum ItemType {
        NYSSE, PASSENGER, STOP, PLAYER, BULLET, NONE
    };

    /**
     * @brief The vec2DF structdefines a 2-dimensional vector with floating-point accuracy, and operations for it
     */
    struct vec2DF
    {
        float x = 0.0f;
        float y = 0.0f;

        void operator+=(const vec2DF& rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
        }

        void operator-=(const vec2DF& rhs) {
            this->x -= rhs.x;
            this->y -= rhs.y;
        }

        vec2DF operator+(const vec2DF& rhs) const {
            return vec2DF{this->x - rhs.x, this->y - rhs.y};
        }

        vec2DF operator-(const vec2DF& rhs) const {
            return vec2DF{this->x - rhs.x, this->y - rhs.y};
        }

        void operator*=(const float& scalar) {
            this->x *= scalar;
            this->y *= scalar;
        }

        void operator/=(const float& scalar) {
            this->x /= scalar;
            this->y /= scalar;
        }

        vec2DF operator*(const float& scalar) const {
            return {this->x * scalar, this->y * scalar};
        }

        vec2DF operator/(const float& scalar) const {
            return {this->x / scalar, this->y / scalar};
        }

        bool operator<(const vec2DF& rhs) const {
            return (std::sqrt(this->x*this->x + this->y+this->y) < std::sqrt(rhs.x*rhs.x + rhs.y*rhs.y));
        }

        bool operator>(const vec2DF& rhs) const {
            return !(*this<rhs);
        }

        bool operator==(const vec2DF& rhs) const {
            return (this->x == rhs.x && this->y == rhs.y);
        }
    };
}

#endif // TYPES_HH
