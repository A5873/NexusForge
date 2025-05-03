/**
 * @file entity.hpp
 * @brief Defines the Entity class for the NexusForge ECS.
 * 
 * This file contains the Entity class which is responsible for managing
 * components and providing a unified interface for game objects.
 */

#ifndef NEXUSFORGE_CORE_ECS_ENTITY_HPP
#define NEXUSFORGE_CORE_ECS_ENTITY_HPP

#include "nexusforge/core/ecs/component.hpp"

#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <cassert>
#include <vector>
#include <algorithm>
#include <string>

namespace nf {

// Forward declarations
class World;

/**
 * @brief Identifier for an entity
 */
using EntityID = std::uint64_t;

/**
 * @brief Entity version type for tracking entity lifecycle
 */
using EntityVersion = std::uint32_t;

/**
 * @brief Invalid entity ID constant
 */
constexpr EntityID INVALID_ENTITY_ID = 0;

/**
 * @brief Class representing an entity in the ECS architecture.
 * 
 * An entity is essentially a container for components that define
 * its behavior and characteristics. Entities are identified by a
 * unique ID and can be activated/deactivated.
 */
class Entity {
public:
    /**
     * @brief Default constructor for an invalid entity
     */
    Entity() : m_id(INVALID_ENTITY_ID), m_version(0), m_active(false), m_world(nullptr) {}
    
    /**
     * @brief Constructor with ID and owner world
     * 
     * @param id The unique identifier for this entity
     * @param version The entity version for safe entity handling
     * @param world Pointer to the world this entity belongs to
     */
    Entity(EntityID id, EntityVersion version, World* world) 
        : m_id(id), m_version(version), m_active(true), m_world(world) {}
    
    /**
     * @brief Destructor that cleans up all components
     */
    ~Entity() {
        removeAllComponents();
    }
    
    /**
     * @brief Check if this entity is valid
     * 
     * An entity is valid if it has a non-zero ID and is associated with a world.
     * 
     * @return True if the entity is valid, false otherwise
     */
    bool isValid() const {
        return m_id != INVALID_ENTITY_ID && m_world != nullptr;
    }
    
    /**
     * @brief Get the unique ID of this entity
     * 
     * @return The entity's ID
     */
    EntityID getID() const {
        return m_id;
    }
    
    /**
     * @brief Get the version of this entity
     * 
     * Entity versions help with safe entity references and detecting stale handles.
     * 
     * @return The entity's version
     */
    EntityVersion getVersion() const {
        return m_version;
    }
    
    /**
     * @brief Check if the entity is active
     * 
     * Inactive entities are not processed by systems.
     * 
     * @return True if the entity is active, false otherwise
     */
    bool isActive() const {
        return m_active;
    }
    
    /**
     * @brief Set the entity's active state
     * 
     * @param active Whether the entity should be active
     */
    void setActive(bool active) {
        m_active = active;
    }
    
    /**
     * @brief Add a component to this entity
     * 
     * Creates a new component of type T and attaches it to this entity.
     * If a component of the same type already exists, it will be replaced.
     * 
     * @tparam T The component type to add
     * @tparam Args Argument types for the component constructor
     * @param args Arguments to forward to the component constructor
     * @return Reference to the newly created component
     */
    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        // Create the component
        std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...);
        ComponentTypeID typeID = component->getTypeID();
        
        // Initialize the component
        component->initialize(*this);
        
        // Store the component
        T& componentRef = *component;
        m_components[typeID] = std::move(component);
        
        return componentRef;
    }
    
    /**
     * @brief Check if this entity has a component of type T
     * 
     * @tparam T The component type to check for
     * @return True if the entity has the component, false otherwise
     */
    template<typename T>
    bool hasComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        return m_components.find(typeID) != m_components.end();
    }
    
    /**
     * @brief Get a component of type T
     * 
     * @tparam T The component type to get
     * @return Reference to the component
     * @throws std::runtime_error if the component doesn't exist
     */
    template<typename T>
    T& getComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        auto it = m_components.find(typeID);
        
        if (it == m_components.end()) {
            throw std::runtime_error("Component not found on entity");
        }
        
        return *static_cast<T*>(it->second.get());
    }
    
    /**
     * @brief Get a component of type T (const version)
     * 
     * @tparam T The component type to get
     * @return Const reference to the component
     * @throws std::runtime_error if the component doesn't exist
     */
    template<typename T>
    const T& getComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        auto it = m_components.find(typeID);
        
        if (it == m_components.end()) {
            throw std::runtime_error("Component not found on entity");
        }
        
        return *static_cast<T*>(it->second.get());
    }
    
    /**
     * @brief Try to get a component of type T
     * 
     * @tparam T The component type to get
     * @return Pointer to the component, or nullptr if not found
     */
    template<typename T>
    T* tryGetComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        auto it = m_components.find(typeID);
        
        if (it == m_components.end()) {
            return nullptr;
        }
        
        return static_cast<T*>(it->second.get());
    }
    
    /**
     * @brief Try to get a component of type T (const version)
     * 
     * @tparam T The component type to get
     * @return Const pointer to the component, or nullptr if not found
     */
    template<typename T>
    const T* tryGetComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        auto it = m_components.find(typeID);
        
        if (it == m_components.end()) {
            return nullptr;
        }
        
        return static_cast<T*>(it->second.get());
    }
    
    /**
     * @brief Remove a component of type T
     * 
     * @tparam T The component type to remove
     * @return True if the component was removed, false if it wasn't found
     */
    template<typename T>
    bool removeComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        ComponentTypeID typeID = ComponentTraits<T>::typeID();
        auto it = m_components.find(typeID);
        
        if (it == m_components.end()) {
            return false;
        }
        
        // Call cleanup on the component
        it->second->cleanup(*this);
        
        // Remove the component
        m_components.erase(it);
        return true;
    }
    
    /**
     * @brief Remove all components from this entity
     */
    void removeAllComponents() {
        // Call cleanup on all components
        for (auto& pair : m_components) {
            pair.second->cleanup(*this);
        }
        
        m_components.clear();
    }
    
    /**
     * @brief Check if this entity has a component with the given type ID
     * 
     * @param typeID The component type ID to check for
     * @return True if the entity has the component, false otherwise
     */
    bool hasComponentWithID(ComponentTypeID typeID) const {
        return m_components.find(typeID) != m_components.end();
    }
    
    /**
     * @brief Get a component by type ID
     * 
     * @param typeID The component type ID to get
     * @return Pointer to the component, or nullptr if not found
     */
    Component* getComponentByID(ComponentTypeID typeID) {
        auto it = m_components.find(typeID);
        return (it != m_components.end()) ? it->second.get() : nullptr;
    }
    
    /**
     * @brief Get a component by type ID (const version)
     * 
     * @param typeID The component type ID to get
     * @return Const pointer to the component, or nullptr if not found
     */
    const Component* getComponentByID(ComponentTypeID typeID) const {
        auto it = m_components.find(typeID);
        return (it != m_components.end()) ? it->second.get() : nullptr;
    }
    
    /**
     * @brief Get the world this entity belongs to
     * 
     * @return Pointer to the owning world
     */
    World* getWorld() const {
        return m_world;
    }
    
    /**
     * @brief Check if two entity references refer to the same entity
     * 
     * @param other The entity to compare with
     * @return True if both references refer to the same entity
     */
    bool operator==(const Entity& other) const {
        return m_id == other.m_id && m_version == other.m_version;
    }
    
    /**
     * @brief Check if two entity references refer to different entities
     * 
     * @param other The entity to compare with
     * @return True if the references refer to different entities
     */
    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

private:
    EntityID m_id;                  ///< Unique identifier for this entity
    EntityVersion m_version;        ///< Entity version for handle validation
    bool m_active;                  ///< Whether this entity is active
    World* m_world;                 ///< The world this entity belongs to
    
    /// Storage for components, mapped by their type ID
    std::unordered_map<ComponentTypeID, std::unique_ptr<Component>> m_components;
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_ENTITY_HPP

