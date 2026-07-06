#pragma once

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <utility>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using Vec2 = glm::vec2;   // Public 2D vector type
using Color = glm::vec4;  // Public color type
using Vec4 = glm::vec4;   // Public vector4 type
using iVec2 = glm::ivec2; // Public 2D integer vector type

template <typename T>
using Array = std::vector<T>; // Public dynamic array type

inline float Sin(float radians)
{
    return std::sin(radians);
}

inline float Cos(float radians)
{
    return std::cos(radians);
}

inline float Tan(float radians)
{
    return std::tan(radians);
}

inline float Atan2(float y, float x)
{
    return std::atan2(y, x);
}

inline float Degrees(float radians)
{
    return radians * 57.29577951308232f;
}

inline float Radians(float degrees)
{
    return degrees * 0.017453292519943295f;
}

inline Vec2 Normalize(const Vec2 &value)
{
    // Returns a unit-length copy of the vector, or zero if the input has no length
    const float length = std::sqrt(value.x * value.x + value.y * value.y);
    if (length <= 0.0f)
    {
        return Vec2(0.0f, 0.0f);
    }

    return value / length;
}

inline std::string ToString(const Vec2 &value)
{
    // Formats a 2D vector for debug text or HUD labels
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "(%.2f, %.2f)", value.x, value.y);
    return buffer;
}

inline std::string ToString(const Vec4 &value)
{
    // Formats a 4D vector for debug text or HUD labels
    char buffer[96];
    std::snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f, %.2f)", value.x, value.y, value.z, value.w);
    return buffer;
}

inline std::string ToString(const iVec2 &value)
{
    // Formats an integer vector for debug text or HUD labels
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "(%d, %d)", value.x, value.y);
    return buffer;
}

inline std::string ToString(double value)
{
    // Formats a time value with one decimal place for simple HUD text
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.1f", value);
    return buffer;
}

template <typename T, typename Predicate>
void EraseIf(Array<T> &values, Predicate predicate)
{
    // Removes every element that matches the predicate
    values.erase(std::remove_if(values.begin(), values.end(), predicate), values.end());
}

template <typename T, typename Predicate>
void RemoveIf(Array<T> &values, Predicate predicate)
{
    // Removes every element that matches the predicate
    EraseIf(values, predicate);
}

template <typename T>
void Add(Array<T> &values, const T &value)
{
    // Adds an element to the end of the array
    values.push_back(value);
}

template <typename T>
void Add(Array<T> &values, T &&value)
{
    // Adds an element to the end of the array
    values.push_back(std::move(value));
}

template <typename T, typename Func>
void ForEach(Array<T> &values, Func func)
{
    // Applies a function to every element in the array
    for (T &value : values)
    {
        func(value);
    }
}

template <typename T, typename Func>
void ForEach(const Array<T> &values, Func func)
{
    // Applies a read-only function to every element in the array
    for (const T &value : values)
    {
        func(value);
    }
}
