/**
 * @file world.hpp
 * @brief Defines the World class for the NexusForge ECS.
 * 
 * This file contains the World class which manages entities and systems,
 * and orchestrates the ECS update cycle.
 */

#ifndef NEXUSFORGE_CORE_ECS_WORLD_HPP
#define NEXUSFORGE_CORE_ECS_WORLD_HPP

#include "nexusforge/core/ecs/component.hpp"
#include "nexusforge/core/ecs/entity.hpp"
#include "nexusforge/core/ecs/system.hpp"

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

namespace nf {

/**
 * @brief Core class that manages entities and systems in the ECS architecture.
 * 
 * The World class is responsible for creating and destroying entities,
 * managing systems, and orchestrating the update cycle. It serves as the
 * central point of interaction for the ECS.
 */
class World {
public:
    /**
     * @brief Constructor
     */
    World()
        : m_nextEntityID(1)
        , m_systemsDirty(false) {}
    
    /**
     * @brief Destructor
     * 
     * Cleans up all entities and systems.
     */
    ~World() {
        destroyAllEntities();
        removeAllSystems();
    }
    
    /**
     * @brief Create a new entity
     * 
     * @return A new entity instance
     */
    Entity createEntity() {
        EntityID id;
        EntityVersion version = 1;
        
        // Reuse ID from destroyed entities when available
        if (!m_freeEntityIDs.empty()) {
            id = m_freeEntityIDs.front();
            m_freeEntityIDs.pop();
            
            // Increment version for recycled IDs
            auto versionIt = m_entityVersions.find(id);
            if (versionIt != m_entityVersions.end()) {
                version = versionIt->second + 1;
                versionIt->second = version;
            } else {
                m_entityVersions[id] = version;
            }
        } else {
            id = m_nextEntityID++;
            m_entityVersions[id] = version;
        }
        
        // Create entity instance
        Entity entity(id, version, this);
        m_entities[id] = entity;
        
        return entity;
    }
    
    /**
     * @brief Destroy an entity
     * 
     * @param entity The entity to destroy
     * @return True if the entity was destroyed, false if it wasn't found
     */
    bool destroyEntity(const Entity& entity) {
        if (!entity.isValid()) {
            return false;
        }
        
        EntityID id = entity.getID();
        auto it = m_entities.find(id);
        
        if (it == m_entities.end() || it->second.getVersion() != entity.getVersion()) {
            return false;  // Entity not found or version mismatch
        }
        
        // Remove entity and recycle ID
        m_entities.erase(it);
        m_freeEntityIDs.push(id);
        
        return true;
    }
    
    /**
     * @brief Destroy all entities
     */
    void destroyAllEntities() {
        m_entities.clear();
        
        // Clear the free ID queue and reset entity ID counter
        while (!m_freeEntityIDs.empty()) {
            m_freeEntityIDs.pop();
        }
        
        m_nextEntityID = 1;
        m_entityVersions.clear();
    }
    
    /**
     * @brief Get an entity by its ID
     * 
     * @param id The entity ID to look up
     * @return The entity if found, an invalid entity otherwise
     */
    Entity getEntity(EntityID id) const {
        auto it = m_entities.find(id);
        return (it != m_entities.end()) ? it->second : Entity();
    }
    
    /**
     * @brief Check if an entity exists and is valid
     * 
     * @param entity The entity to check
     * @return True if the entity exists and has the correct version
     */
    bool isEntityValid(const Entity& entity) const {
        if (!entity.isValid()) {
            return false;
        }
        
        auto it = m_entities.find(entity.getID());
        return (it != m_entities.end() && it->second.getVersion() == entity.getVersion());
    }
    
    /**
     * @brief Get the number of entities in the world
     * 
     * @return The entity count
     */
    size_t getEntityCount() const {
        return m_entities.size();
    }
    
    /**
     * @brief Add a system to the world
     * 
     * @tparam T The system type
     * @tparam Args Argument types for the system constructor
     * @param args Arguments to forward to the system constructor
     * @return Reference to the newly added system
     */
    template<typename T, typename... Args>
    T& addSystem(Args&&... args) {
        static_assert(std::is_base_of<System, T>::value, "T must derive from System");
        
        // Create the system
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T& systemRef = *system;
        
        // Initialize the system
        if (!system->initialize(*this)) {
            throw std::runtime_error("Failed to initialize system: " + system->getName());
        }
        
        // Store the system
        m_systems.push_back(std::move(system));
        m_systemsDirty = true;
        
        return systemRef;
    }
    
    /**
     * @brief Remove a system by type
     * 
     * @tparam T The system type to remove
     * @return True if a system was removed, false otherwise
     */
    template<typename T>
    bool removeSystem() {
        static_assert(std::is_base_of<System, T>::value, "T must derive from System");
        
        for (auto it = m_systems.begin(); it != m_systems.end(); ++it) {
            if (dynamic_cast<T*>(it->get()) != nullptr) {
                // Call cleanup on the system
                (*it)->cleanup();
                
                // Remove the system
                m_systems.erase(it);
                m_systemsDirty = true;
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief Get a system by type
     * 
     * @tparam T The system type to get
     * @return Pointer to the system, or nullptr if not found
     */
    template<typename T>
    T* getSystem() const {
        static_assert(std::is_base_of<System, T>::value, "T must derive from System");
        
        for (const auto& system : m_systems) {
            if (auto typedSystem = dynamic_cast<T*>(system.get())) {
                return typedSystem;
            }
        }
        
        return nullptr;
    }
    
    /**
     * @brief Remove all systems
     */
    void removeAllSystems() {
        // Call cleanup on all systems
        for (auto& system : m_systems) {
            system->cleanup();
        }
        
        m_systems.clear();
        m_systemsDirty = false;
    }
    
    /**
     * @brief Update all systems
     * 
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime) {
        // Sort systems by priority if needed
        if (m_systemsDirty) {
            sortSystems();
        }
        
        // Update all enabled systems
        for (auto& system : m_systems) {
            if (system->isEnabled()) {
                system->update(deltaTime);
            }
        }
    }
    
    /**
     * @brief Find entities with specific component types
     * 
     * @tparam T Component type(s) to match
     * @return Vector of entities that have the specified component types
     */
    template<typename... T>
    std::vector<Entity> findEntities() const {
        std::vector<Entity> result;
        std::array<ComponentTypeID, sizeof...(T)> componentIDs = {ComponentTraits<T>::typeID()...};
        
        for (const auto& pair : m_entities) {
            const Entity& entity = pair.second;
            
            if (!entity.isActive()) {
                continue;
            }
            
            bool match = true;
            for (ComponentTypeID typeID : componentIDs) {
                if (!entity.hasComponentWithID(typeID)) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                result.push_back(entity);
            }
        }
        
        return result;
    }
    
    /**
     * @brief Find entities matching a custom predicate
     * 
     * @param predicate Function that takes an Entity and returns a bool
     * @return Vector of entities that match the predicate
     */
    std::vector<Entity> findEntitiesWithPredicate(const std::function<bool(const Entity&)>& predicate) const {
        std::vector<Entity> result;
        
        for (const auto& pair : m_entities) {
            const Entity& entity = pair.second;
            
            if (entity.isActive() && predicate(entity)) {
                result.push_back(entity);
            }
        }
        
        return result;
    }
    
    /**
     * @brief Process each entity with specific component types
     * 
     * @tparam T Component type(s) to match
     * @param func Function to call for each matching entity
     */
    template<typename... T>
    void forEachEntity(const std::function<void(Entity&)>& func) {
        std::array<ComponentTypeID, sizeof...(T)> componentIDs = {ComponentTraits<T>::typeID()...};
        
        for (auto& pair : m_entities) {
            Entity& entity = pair.second;
            
            if (!entity.isActive()) {
                continue;
            }
            
            bool match = true;
            for (ComponentTypeID typeID : componentIDs) {
                if (!entity.hasComponentWithID(typeID)) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                func(entity);
            }
        }
    }
    
    /**
     * @brief Get the number of systems in the world
     * 
     * @return The system count
     */
    size_t getSystemCount() const {
        return m_systems.size();
    }

private:
    /**
     * @brief Sort systems by priority
     * 
     * Higher priority systems will be updated first.
     */
    void sortSystems() {
        std::sort(m_systems.begin(), m_systems.end(), SystemPriorityCompare());
        m_systemsDirty = false;
    }
    
    EntityID m_nextEntityID;                               ///< Next entity ID to assign
    std::queue<EntityID> m_freeEntityIDs;                  ///< Queue of recyclable entity IDs
    std::unordered_map<EntityID, EntityVersion> m_entityVersions; ///< Map of entity versions
    std::unordered_map<EntityID, Entity> m_entities;       ///< Map of active entities
    std::vector<std::unique_ptr<System>> m_systems;        ///< List of systems
    bool m_systemsDirty;                                   ///< Flag for system sort needed
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_WORLD_HPP

