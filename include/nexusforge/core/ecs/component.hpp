/**
 * @file component.hpp
 * @brief Defines the component interface for the NexusForge ECS.
 * 
 * This file contains the base Component class and related utilities for the
 * Entity Component System (ECS) architecture within NexusForge.
 */

#ifndef NEXUSFORGE_CORE_ECS_COMPONENT_HPP
#define NEXUSFORGE_CORE_ECS_COMPONENT_HPP

#include <cstdint>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace nf {

// Forward declarations
class Entity;

/**
 * @brief Component type identifier
 * 
 * Used to uniquely identify component types at runtime
 */
using ComponentTypeID = std::uint32_t;

/**
 * @brief Base class for all components in the ECS architecture.
 * 
 * Components are data containers that hold specific aspects of entity state.
 * They should be lightweight, focused on data rather than behavior, and 
 * easily composable.
 */
class Component {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived components
     */
    virtual ~Component() = default;

    /**
     * @brief Initialize the component
     * 
     * Called when the component is first attached to an entity.
     * @param entity The entity this component is attached to
     * @return True if initialization succeeded, false otherwise
     */
    virtual bool initialize(Entity& entity) { return true; }

    /**
     * @brief Clean up component resources
     * 
     * Called when the component is about to be removed from an entity.
     * @param entity The entity this component is attached to
     */
    virtual void cleanup(Entity& entity) {}

    /**
     * @brief Get the type ID of this component instance
     * 
     * @return The runtime component type ID
     */
    virtual ComponentTypeID getTypeID() const = 0;

    /**
     * @brief Check if this component is enabled
     * 
     * @return True if the component is enabled, false otherwise
     */
    bool isEnabled() const { return m_enabled; }

    /**
     * @brief Enable or disable this component
     * 
     * @param enabled Whether the component should be enabled
     */
    void setEnabled(bool enabled) { m_enabled = enabled; }

protected:
    // Components should only be created through the entity interface
    Component() = default;

private:
    bool m_enabled = true;
};

/**
 * @brief Component type ID generation utilities
 */
namespace detail {
    // Starting ID for user components
    constexpr ComponentTypeID BASE_COMPONENT_ID = 1000;
    
    // Global component type counter for automatic ID assignment
    inline ComponentTypeID g_componentTypeCounter = BASE_COMPONENT_ID;

    /**
     * @brief Get the next available component type ID
     * 
     * @return A unique component type ID
     */
    inline ComponentTypeID getNextComponentTypeID() {
        return g_componentTypeCounter++;
    }

    /**
     * @brief Get a unique component type ID for type T
     * 
     * This template uses static variable instantiation to ensure
     * each component type receives a unique ID.
     * 
     * @tparam T The component type
     * @return The unique type ID for component type T
     */
    template<typename T>
    ComponentTypeID getComponentTypeID() {
        static_assert(std::is_base_of<Component, T>::value, 
                    "T must derive from Component");
        
        static const ComponentTypeID typeID = getNextComponentTypeID();
        return typeID;
    }
}

/**
 * @brief Helper macro to define a component type
 * 
 * This macro handles the boilerplate of implementing the getTypeID() method
 * for component classes.
 */
#define NF_COMPONENT(ComponentClass) \
public: \
    static ComponentTypeID staticTypeID() { \
        return nf::detail::getComponentTypeID<ComponentClass>(); \
    } \
    virtual ComponentTypeID getTypeID() const override { \
        return staticTypeID(); \
    }

/**
 * @brief Traits for working with component types
 * 
 * Provides type identification and other type-level utilities
 * for component classes.
 */
template<typename T>
struct ComponentTraits {
    static constexpr bool is_component = std::is_base_of<Component, T>::value;

    /**
     * @brief Get the static type ID for component type T
     * 
     * @return The unique type ID for component type T
     */
    static ComponentTypeID typeID() {
        static_assert(is_component, "T must derive from Component");
        return detail::getComponentTypeID<T>();
    }
};

} // namespace nf

#endif // NEXUSFORGE_CORE_ECS_COMPONENT_HPP

