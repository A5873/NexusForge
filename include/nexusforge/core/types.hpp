#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

// Ensure proper export macros for shared library builds
#if defined(_MSC_VER) && defined(NF_SHARED_LIBRARY)
    #ifdef NF_EXPORT
        #define NF_API __declspec(dllexport)
    #else
        #define NF_API __declspec(dllimport)
    #endif
#else
    #define NF_API
#endif

namespace nf {

// Common type definitions
using EntityId = std::uint64_t;
using ComponentId = std::uint32_t;
using SystemId = std::uint32_t;
using ResourceId = std::uint64_t;

// Common constants
constexpr float PI = 3.14159265358979323846f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

// 2D/3D agnostic types
#ifdef NF_DIMENSION_3D
    struct Vec2 {
        float x, y;
        
        Vec2() : x(0.0f), y(0.0f) {}
        Vec2(float x, float y) : x(x), y(y) {}
        
        // Utility functions
        float length() const { return std::sqrt(x*x + y*y); }
        Vec2 normalized() const {
            float len = length();
            return len > 0 ? Vec2(x/len, y/len) : Vec2();
        }
    };

    struct Vec3 {
        float x, y, z;
        
        Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        // Allow implicit conversion from Vec2 for dimensional compatibility
        Vec3(const Vec2& v) : x(v.x), y(v.y), z(0.0f) {}
        
        // Utility functions
        float length() const { return std::sqrt(x*x + y*y + z*z); }
        Vec3 normalized() const {
            float len = length();
            return len > 0 ? Vec3(x/len, y/len, z/len) : Vec3();
        }
    };

    // Default to 3D vector
    using Vector = Vec3;
#else
    struct Vec2 {
        float x, y;
        
        Vec2() : x(0.0f), y(0.0f) {}
        Vec2(float x, float y) : x(x), y(y) {}
        
        // Utility functions
        float length() const { return std::sqrt(x*x + y*y); }
        Vec2 normalized() const {
            float len = length();
            return len > 0 ? Vec2(x/len, y/len) : Vec2();
        }
    };

    // Default to 2D vector
    using Vector = Vec2;
#endif

// Result type for error handling
enum class Result {
    Success,
    Failure,
    NotImplemented,
    InvalidArgument,
    ResourceNotFound
};

// String conversion for Result
inline std::string resultToString(Result result) {
    switch (result) {
        case Result::Success: return "Success";
        case Result::Failure: return "Failure";
        case Result::NotImplemented: return "NotImplemented";
        case Result::InvalidArgument: return "InvalidArgument";
        case Result::ResourceNotFound: return "ResourceNotFound";
        default: return "Unknown";
    }
}

// Version information
struct Version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    
    std::string toString() const {
        return std::to_string(major) + "." + 
               std::to_string(minor) + "." + 
               std::to_string(patch);
    }
    
    bool operator==(const Version& other) const {
        return major == other.major && minor == other.minor && patch == other.patch;
    }
    
    bool operator!=(const Version& other) const {
        return !(*this == other);
    }
    
    bool operator>(const Version& other) const {
        if (major != other.major) return major > other.major;
        if (minor != other.minor) return minor > other.minor;
        return patch > other.patch;
    }
    
    bool operator<(const Version& other) const {
        return other > *this;
    }
    
    bool operator>=(const Version& other) const {
        return *this > other || *this == other;
    }
    
    bool operator<=(const Version& other) const {
        return *this < other || *this == other;
    }
};

} // namespace nf

