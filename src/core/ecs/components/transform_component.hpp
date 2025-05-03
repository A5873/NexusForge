/**
 * @file transform_component.hpp
 * @brief Defines the TransformComponent for the NexusForge ECS.
 * 
 * This file contains the TransformComponent which handles 2D transformations
 * for entities, including position, rotation, and scale.
 */

#ifndef NEXUSFORGE_CORE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP
#define NEXUSFORGE_CORE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

#include "nexusforge/core/ecs/component.hpp"
#include "nexusforge/core/ecs/entity.hpp"
#include <cmath>
#include <array>

namespace nf {

/**
 * @brief Simple 2D vector class for position and scale
 */
struct Vector2 {
    float x;
    float y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Vector addition
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    // Vector subtraction
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    // Scalar multiplication
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // Length of vector
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector
    Vector2 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector2(x / len, y / len);
        }
        return *this;
    }

    // Distance to another vector
    float distance(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

/**
 * @brief Component that handles 2D transformations for entities.
 * 
 * The TransformComponent maintains an entity's position, rotation, and scale
 * in 2D space. It provides methods for manipulating these properties and
 * performing common transformations.
 */
class TransformComponent : public Component {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a transform at the origin with no rotation and unit scale.
     */
    TransformComponent()
        : m_position(0.0f, 0.0f)
        , m_rotation(0.0f)
        , m_scale(1.0f, 1.0f)
        , m_dirty(true) {}
    
    /**
     * @brief Constructor with position
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    TransformComponent(float x, float y)
        : m_position(x, y)
        , m_rotation(0.0f)
        , m_scale(1.0f, 1.0f)
        , m_dirty(true) {}
    
    /**
     * @brief Constructor with position vector
     * 
     * @param position Initial position
     */
    TransformComponent(const Vector2& position)
        : m_position(position)
        , m_rotation(0.0f)
        , m_scale(1.0f, 1.0f)
        , m_dirty(true) {}
    
    /**
     * @brief Constructor with position, rotation, and scale
     * 
     * @param position Initial position
     * @param rotation Initial rotation in radians
     * @param scale Initial scale
     */
    TransformComponent(const Vector2& position, float rotation, const Vector2& scale)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale)
        , m_dirty(true) {}

    /**
     * @brief Initialize the component
     * 
     * @param entity The entity this component is attached to
     * @return True if initialization succeeded
     */
    bool initialize(Entity& entity) override {
        // No special initialization needed
        return true;
    }
    
    /**
     * @brief Get the position
     * 
     * @return Current position
     */
    const Vector2& getPosition() const {
        return m_position;
    }
    
    /**
     * @brief Set the position
     * 
     * @param position New position
     */
    void setPosition(const Vector2& position) {
        m_position = position;
        m_dirty = true;
    }
    
    /**
     * @brief Set the position
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    void setPosition(float x, float y) {
        m_position.x = x;
        m_position.y = y;
        m_dirty = true;
    }
    
    /**
     * @brief Get the X position
     * 
     * @return X-coordinate
     */
    float getX() const {
        return m_position.x;
    }
    
    /**
     * @brief Set the X position
     * 
     * @param x New X-coordinate
     */
    void setX(float x) {
        m_position.x = x;
        m_dirty = true;
    }
    
    /**
     * @brief Get the Y position
     * 
     * @return Y-coordinate
     */
    float getY() const {
        return m_position.y;
    }
    
    /**
     * @brief Set the Y position
     * 
     * @param y New Y-coordinate
     */
    void setY(float y) {
        m_position.y = y;
        m_dirty = true;
    }
    
    /**
     * @brief Get the rotation
     * 
     * @return Current rotation in radians
     */
    float getRotation() const {
        return m_rotation;
    }
    
    /**
     * @brief Set the rotation
     * 
     * @param rotation New rotation in radians
     */
    void setRotation(float rotation) {
        m_rotation = rotation;
        m_dirty = true;
    }
    
    /**
     * @brief Get the scale
     * 
     * @return Current scale
     */
    const Vector2& getScale() const {
        return m_scale;
    }
    
    /**
     * @brief Set the scale
     * 
     * @param scale New scale
     */
    void setScale(const Vector2& scale) {
        m_scale = scale;
        m_dirty = true;
    }
    
    /**
     * @brief Set the scale
     * 
     * @param x X-scale
     * @param y Y-scale
     */
    void setScale(float x, float y) {
        m_scale.x = x;
        m_scale.y = y;
        m_dirty = true;
    }
    
    /**
     * @brief Set a uniform scale
     * 
     * Sets both X and Y scale to the same value.
     * 
     * @param scale New uniform scale
     */
    void setScale(float scale) {
        m_scale.x = scale;
        m_scale.y = scale;
        m_dirty = true;
    }
    
    /**
     * @brief Move the transform by a delta
     * 
     * @param delta Movement vector to add to the current position
     */
    void move(const Vector2& delta) {
        m_position = m_position + delta;
        m_dirty = true;
    }
    
    /**
     * @brief Move the transform by a delta
     * 
     * @param dx X-delta
     * @param dy Y-delta
     */
    void move(float dx, float dy) {
        m_position.x += dx;
        m_position.y += dy;
        m_dirty = true;
    }
    
    /**
     * @brief Rotate the transform by a delta
     * 
     * @param deltaRadians Rotation delta in radians
     */
    void rotate(float deltaRadians) {
        m_rotation += deltaRadians;
        m_dirty = true;
    }
    
    /**
     * @brief Scale the transform by a factor
     * 
     * @param factor Scale factor to multiply by the current scale
     */
    void scale(const Vector2& factor) {
        m_scale.x *= factor.x;
        m_scale.y *= factor.y;
        m_dirty = true;
    }
    
    /**
     * @brief Scale the transform by a factor
     * 
     * @param factorX X-scale factor
     * @param factorY Y-scale factor
     */
    void scale(float factorX, float factorY) {
        m_scale.x *= factorX;
        m_scale.y *= factorY;
        m_dirty = true;
    }
    
    /**
     * @brief Scale the transform uniformly by a factor
     * 
     * @param factor Uniform scale factor
     */
    void scale(float factor) {
        m_scale.x *= factor;
        m_scale.y *= factor;
        m_dirty = true;
    }
    
    /**
     * @brief Check if the transform has changed since last access
     * 
     * @return True if the transform is dirty
     */
    bool isDirty() const {
        return m_dirty;
    }
    
    /**
     * @brief Mark the transform as clean
     * 
     * Call this after processing the transform changes.
     */
    void clearDirty() {
        m_dirty = false;
    }
    
    /**
     * @brief Get the transformation matrix for this transform
     * 
     * Returns a 3x3 transformation matrix in column-major order.
     * 
     * @return Array representing the transformation matrix
     */
    std::array<float, 9> getTransformMatrix() const {
        float cosR = std::cos(m_rotation);
        float sinR = std::sin(m_rotation);
        
        // Column-major order (OpenGL style)
        return {
            m_scale.x * cosR,  m_scale.x * sinR,  0.0f,
            -m_scale.y * sinR, m_scale.y * cosR,  0.0f,
            m_position.x,      m_position.y,      1.0f
        };
    }
    
    /**
     * @brief Transform a point from local to world space
     * 
     * @param localPoint Point in local space
     * @return Point in world space
     */
    Vector2 transformPoint(const Vector2& localPoint) const {
        float cosR = std::cos(m_rotation);
        float sinR = std::sin(m_rotation);
        
        float x = localPoint.x * m_scale.x;
        float y = localPoint.y * m_scale.y;
        
        return Vector2(
            x * cosR - y * sinR + m_position.x,
            x * sinR + y * cosR + m_position.y
        );
    }

    // Register this class as a component
    NF_COMPONENT(TransformComponent)
    
private:
    Vector2 m_position;  ///< Position in 2D space
    float m_rotation;    ///< Rotation in radians
    Vector2 m_scale;     ///< Scale in X and Y
    bool m_dirty;        ///< Flag indicating the transform has changed
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_COMPONENTS_TRANSFORM_COMPONENT_HPP

