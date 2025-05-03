/**
 * @file system.hpp
 * @brief Defines the System base class for the NexusForge ECS.
 * 
 * This file contains the System base class which defines the interface for
 * processing entities with specific component combinations.
 */

#ifndef NEXUSFORGE_CORE_ECS_SYSTEM_HPP
#define NEXUSFORGE_CORE_ECS_SYSTEM_HPP

#include "nexusforge/core/ecs/component.hpp"
#include "nexusforge/core/ecs/entity.hpp"

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

namespace nf {

// Forward declarations
class World;

/**
 * @brief System priority type
 * 
 * Higher priority systems run before lower priority systems.
 */
using SystemPriority = int;

/**
 * @brief Component type set for tracking component requirements
 */
using ComponentTypeSet = std::unordered_set<ComponentTypeID>;

/**
 * @brief Default system priorities
 */
namespace SystemPriorities {
    constexpr SystemPriority HIGHEST = 1000;
    constexpr SystemPriority HIGH = 750;
    constexpr SystemPriority NORMAL = 500;
    constexpr SystemPriority LOW = 250;
    constexpr SystemPriority LOWEST = 0;
}

/**
 * @brief Base class for all systems in the ECS architecture.
 * 
 * Systems process entities with specific component combinations and
 * implement game logic. They define what types of components are required
 * for processing and how entities should be updated.
 */
class System {
public:
    /**
     * @brief Constructor with optional system name and priority
     * 
     * @param name Name of the system
     * @param priority Priority of the system (higher = executed earlier)
     */
    explicit System(const std::string& name = "UnnamedSystem", 
                   SystemPriority priority = SystemPriorities::NORMAL)
        : m_name(name)
        , m_priority(priority)
        , m_enabled(true)
        , m_world(nullptr) {}
    
    /**
     * @brief Virtual destructor
     */
    virtual ~System() = default;
    
    /**
     * @brief Initialize the system
     * 
     * Called when the system is first added to the world.
     * 
     * @param world The world this system belongs to
     * @return True if initialization succeeded, false otherwise
     */
    virtual bool initialize(World& world) {
        m_world = &world;
        return true;
    }
    
    /**
     * @brief Clean up system resources
     * 
     * Called when the system is about to be removed from the world.
     */
    virtual void cleanup() {
        m_world = nullptr;
    }
    
    /**
     * @brief Update the system
     * 
     * Process all matching entities. This is the main method that derived
     * systems should implement to define their logic.
     * 
     * @param deltaTime Time elapsed since the last update
     */
    virtual void update(float deltaTime) = 0;
    
    /**
     * @brief Check if an entity matches this system's requirements
     * 
     * An entity matches if it has all required components and all additional
     * optional components specified for the system.
     * 
     * @param entity The entity to check
     * @return True if the entity matches the system's requirements
     */
    virtual bool matchesEntity(const Entity& entity) const {
        // Entity must be valid and active
        if (!entity.isValid() || !entity.isActive()) {
            return false;
        }
        
        // Check required components
        for (ComponentTypeID typeID : m_requiredComponents) {
            if (!entity.hasComponentWithID(typeID)) {
                return false;
            }
        }
        
        // Check optional components (entity must have all specified optional components)
        for (ComponentTypeID typeID : m_optionalComponents) {
            if (!entity.hasComponentWithID(typeID)) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief Get the name of the system
     * 
     * @return The system's name
     */
    const std::string& getName() const {
        return m_name;
    }
    
    /**
     * @brief Set the name of the system
     * 
     * @param name The new name
     */
    void setName(const std::string& name) {
        m_name = name;
    }
    
    /**
     * @brief Get the priority of the system
     * 
     * Higher priority systems are processed before lower priority systems.
     * 
     * @return The system's priority
     */
    SystemPriority getPriority() const {
        return m_priority;
    }
    
    /**
     * @brief Set the priority of the system
     * 
     * @param priority The new priority
     */
    void setPriority(SystemPriority priority) {
        m_priority = priority;
    }
    
    /**
     * @brief Check if the system is enabled
     * 
     * Disabled systems are not updated.
     * 
     * @return True if the system is enabled
     */
    bool isEnabled() const {
        return m_enabled;
    }
    
    /**
     * @brief Enable or disable the system
     * 
     * @param enabled Whether the system should be enabled
     */
    void setEnabled(bool enabled) {
        m_enabled = enabled;
    }
    
    /**
     * @brief Get the world this system belongs to
     * 
     * @return Pointer to the owning world
     */
    World* getWorld() const {
        return m_world;
    }
    
    /**
     * @brief Get the set of required component types
     * 
     * @return The set of required component type IDs
     */
    const ComponentTypeSet& getRequiredComponents() const {
        return m_requiredComponents;
    }
    
    /**
     * @brief Get the set of optional component types
     * 
     * @return The set of optional component type IDs
     */
    const ComponentTypeSet& getOptionalComponents() const {
        return m_optionalComponents;
    }

protected:
    /**
     * @brief Add a required component type to this system
     * 
     * Entities must have all required components to be processed by this system.
     * 
     * @tparam T The component type to add as a requirement
     */
    template<typename T>
    void requireComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        m_requiredComponents.insert(ComponentTraits<T>::typeID());
    }
    
    /**
     * @brief Add an optional component type to this system
     * 
     * Optional components are not required for entities to be processed by this system,
     * but can be used if present.
     * 
     * @tparam T The component type to add as optional
     */
    template<typename T>
    void requireOptionalComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        m_optionalComponents.insert(ComponentTraits<T>::typeID());
    }
    
    /**
     * @brief Add a required component type to this system by type ID
     * 
     * @param typeID The component type ID to add as a requirement
     */
    void requireComponentWithID(ComponentTypeID typeID) {
        m_requiredComponents.insert(typeID);
    }
    
    /**
     * @brief Add an optional component type to this system by type ID
     * 
     * @param typeID The component type ID to add as optional
     */
    void requireOptionalComponentWithID(ComponentTypeID typeID) {
        m_optionalComponents.insert(typeID);
    }

private:
    std::string m_name;                 ///< Name of the system
    SystemPriority m_priority;          ///< Priority of the system
    bool m_enabled;                     ///< Whether the system is enabled
    World* m_world;                     ///< The world this system belongs to
    
    ComponentTypeSet m_requiredComponents;  ///< Set of required component types
    ComponentTypeSet m_optionalComponents;  ///< Set of optional component types
};

/**
 * @brief Helper for comparing system pointers by priority
 */
struct SystemPriorityCompare {
    bool operator()(const std::unique_ptr<System>& a, const std::unique_ptr<System>& b) const {
        return a->getPriority() > b->getPriority(); // Higher priority comes first
    }
};

/**
 * @brief Types for working with collections of systems
 */
using SystemPtr = std::unique_ptr<System>;
using SystemList = std::vector<SystemPtr>;

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_SYSTEM_HPP

