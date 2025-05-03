/**
 * @file movement_system.hpp
 * @brief Defines the MovementSystem for the NexusForge ECS.
 * 
 * This file contains the MovementSystem which processes entity movement
 * based on transform and velocity components.
 */

#ifndef NEXUSFORGE_CORE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP
#define NEXUSFORGE_CORE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP

#include "nexusforge/core/ecs/system.hpp"
#include "nexusforge/core/ecs/world.hpp"
#include "../components/transform_component.hpp"
#include "../components/velocity_component.hpp"

#include <string>
#include <iostream>

namespace nf {

/**
 * @brief System that handles entity movement based on velocity.
 * 
 * The MovementSystem updates entity positions and rotations based on their
 * velocity components. It demonstrates basic system implementation and
 * component interactions.
 */
class MovementSystem : public System {
public:
    /**
     * @brief Constructor
     * 
     * @param name System name
     * @param priority System priority
     */
    MovementSystem(const std::string& name = "MovementSystem", 
                   SystemPriority priority = SystemPriorities::NORMAL)
        : System(name, priority)
        , m_linearDamping(0.0f)
        , m_angularDamping(0.0f) {
        
        // Specify required components
        requireComponent<TransformComponent>();
        requireComponent<VelocityComponent>();
    }
    
    /**
     * @brief Initialize the system
     * 
     * @param world The world this system belongs to
     * @return True if initialization succeeded
     */
    bool initialize(World& world) override {
        bool result = System::initialize(world);
        std::cout << "MovementSystem initialized with priority " << getPriority() << std::endl;
        return result;
    }
    
    /**
     * @brief Update entity movement
     * 
     * Processes all entities with transform and velocity components,
     * updating their positions and rotations based on velocity.
     * 
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime) override {
        if (!getWorld()) {
            return;
        }
        
        // Method 1: Using World's entity querying system
        getWorld()->forEachEntity<TransformComponent, VelocityComponent>(
            [this, deltaTime](Entity& entity) {
                updateEntityMovement(entity, deltaTime);
            }
        );
        
        // Method 2: Alternative approach using findEntities
        /*
        auto entities = getWorld()->findEntities<TransformComponent, VelocityComponent>();
        for (auto& entity : entities) {
            updateEntityMovement(entity, deltaTime);
        }
        */
    }
    
    /**
     * @brief Set global damping factors
     * 
     * Sets the damping factors applied to all entities processed by this system.
     * 
     * @param linearDamping Damping factor for linear velocity [0,1]
     * @param angularDamping Damping factor for angular velocity [0,1]
     */
    void setDamping(float linearDamping, float angularDamping) {
        m_linearDamping = linearDamping;
        m_angularDamping = angularDamping;
    }
    
    /**
     * @brief Get the linear damping factor
     * 
     * @return Current linear damping factor
     */
    float getLinearDamping() const {
        return m_linearDamping;
    }
    
    /**
     * @brief Get the angular damping factor
     * 
     * @return Current angular damping factor
     */
    float getAngularDamping() const {
        return m_angularDamping;
    }
    
private:
    /**
     * @brief Update movement for a single entity
     * 
     * @param entity Entity to update
     * @param deltaTime Time elapsed since the last update
     */
    void updateEntityMovement(Entity& entity, float deltaTime) {
        // Get required components
        auto& transform = entity.getComponent<TransformComponent>();
        auto& velocity = entity.getComponent<VelocityComponent>();
        
        // Apply velocity to position
        transform.move(velocity.getVelocity().x * deltaTime, 
                      velocity.getVelocity().y * deltaTime);
        
        // Apply angular velocity to rotation
        transform.rotate(velocity.getAngularVelocity() * deltaTime);
        
        // Apply damping if configured
        if (m_linearDamping > 0.0f || m_angularDamping > 0.0f) {
            velocity.applyDamping(m_linearDamping * deltaTime, 
                                 m_angularDamping * deltaTime);
        }
    }
    
    float m_linearDamping;   ///< Damping factor for linear velocity
    float m_angularDamping;  ///< Damping factor for angular velocity
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_SYSTEMS_MOVEMENT_SYSTEM_HPP

