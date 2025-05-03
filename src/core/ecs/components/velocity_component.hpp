/**
 * @file velocity_component.hpp
 * @brief Defines the VelocityComponent for the NexusForge ECS.
 * 
 * This file contains the VelocityComponent which stores velocity data
 * for entities in 2D space.
 */

#ifndef NEXUSFORGE_CORE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP
#define NEXUSFORGE_CORE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP

#include "nexusforge/core/ecs/component.hpp"
#include "nexusforge/core/ecs/entity.hpp"
#include "transform_component.hpp" // For Vector2

namespace nf {

/**
 * @brief Component that stores velocity data for entities.
 * 
 * The VelocityComponent maintains an entity's linear and angular velocity
 * for use in movement and physics systems.
 */
class VelocityComponent : public Component {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a velocity component with zero velocity.
     */
    VelocityComponent()
        : m_velocity(0.0f, 0.0f)
        , m_angularVelocity(0.0f) {}
    
    /**
     * @brief Constructor with linear velocity
     * 
     * @param x X-component of velocity
     * @param y Y-component of velocity
     */
    VelocityComponent(float x, float y)
        : m_velocity(x, y)
        , m_angularVelocity(0.0f) {}
    
    /**
     * @brief Constructor with linear velocity vector
     * 
     * @param velocity Initial velocity
     */
    VelocityComponent(const Vector2& velocity)
        : m_velocity(velocity)
        , m_angularVelocity(0.0f) {}
    
    /**
     * @brief Constructor with linear and angular velocity
     * 
     * @param velocity Initial linear velocity
     * @param angularVelocity Initial angular velocity in radians per second
     */
    VelocityComponent(const Vector2& velocity, float angularVelocity)
        : m_velocity(velocity)
        , m_angularVelocity(angularVelocity) {}
    
    /**
     * @brief Get the linear velocity
     * 
     * @return Current linear velocity vector
     */
    const Vector2& getVelocity() const {
        return m_velocity;
    }
    
    /**
     * @brief Set the linear velocity
     * 
     * @param velocity New linear velocity
     */
    void setVelocity(const Vector2& velocity) {
        m_velocity = velocity;
    }
    
    /**
     * @brief Set the linear velocity
     * 
     * @param x X-component of velocity
     * @param y Y-component of velocity
     */
    void setVelocity(float x, float y) {
        m_velocity.x = x;
        m_velocity.y = y;
    }
    
    /**
     * @brief Get the angular velocity
     * 
     * @return Current angular velocity in radians per second
     */
    float getAngularVelocity() const {
        return m_angularVelocity;
    }
    
    /**
     * @brief Set the angular velocity
     * 
     * @param angularVelocity New angular velocity in radians per second
     */
    void setAngularVelocity(float angularVelocity) {
        m_angularVelocity = angularVelocity;
    }
    
    /**
     * @brief Add force to the velocity
     * 
     * @param force Force vector to add
     * @param mass Mass of the entity (default = 1.0)
     */
    void addForce(const Vector2& force, float mass = 1.0f) {
        // F = ma, so a = F/m
        m_velocity = m_velocity + force * (1.0f / mass);
    }
    
    /**
     * @brief Add torque to the angular velocity
     * 
     * @param torque Torque to add
     * @param momentOfInertia Moment of inertia (default = 1.0)
     */
    void addTorque(float torque, float momentOfInertia = 1.0f) {
        // τ = Iα, so α = τ/I
        m_angularVelocity += torque / momentOfInertia;
    }
    
    /**
     * @brief Apply damping to velocities
     * 
     * Reduces velocities over time to simulate friction or drag.
     * 
     * @param linearDamping Damping factor for linear velocity [0,1]
     * @param angularDamping Damping factor for angular velocity [0,1]
     */
    void applyDamping(float linearDamping, float angularDamping) {
        m_velocity.x *= (1.0f - linearDamping);
        m_velocity.y *= (1.0f - linearDamping);
        m_angularVelocity *= (1.0f - angularDamping);
    }

    // Register this class as a component
    NF_COMPONENT(VelocityComponent)
    
private:
    Vector2 m_velocity;        ///< Linear velocity
    float m_angularVelocity;   ///< Angular velocity in radians per second
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_COMPONENTS_VELOCITY_COMPONENT_HPP

