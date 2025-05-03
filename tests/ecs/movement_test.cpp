/**
 * @file movement_test.cpp
 * @brief Test for the NexusForge ECS movement system.
 * 
 * This file contains tests that verify the Entity Component System (ECS)
 * implementation, focusing on the TransformComponent, VelocityComponent,
 * and MovementSystem.
 */

#include "nexusforge/core/ecs/world.hpp"
#include "src/core/ecs/components/transform_component.hpp"
#include "src/core/ecs/components/velocity_component.hpp"
#include "src/core/ecs/systems/movement_system.hpp"

#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>

// Helper for floating point comparisons
bool approximatelyEqual(float a, float b, float epsilon = 0.0001f) {
    return std::fabs(a - b) <= epsilon;
}

// Helper for Vector2 comparisons
bool approximatelyEqual(const nf::Vector2& a, const nf::Vector2& b, float epsilon = 0.0001f) {
    return approximatelyEqual(a.x, b.x, epsilon) && approximatelyEqual(a.y, b.y, epsilon);
}

// Helper to print test results
void printTestResult(const std::string& testName, bool success) {
    std::cout << "[" << (success ? "PASS" : "FAIL") << "] " << testName << std::endl;
}

// Helper to print entity position
void printEntityPosition(const nf::Entity& entity, const std::string& label) {
    if (!entity.isValid()) {
        std::cout << label << ": Invalid entity" << std::endl;
        return;
    }
    
    if (!entity.hasComponent<nf::TransformComponent>()) {
        std::cout << label << ": No transform component" << std::endl;
        return;
    }
    
    const auto& transform = entity.getComponent<nf::TransformComponent>();
    const auto& pos = transform.getPosition();
    const auto rotation = transform.getRotation();
    
    std::cout << label << ": Position(" 
              << std::fixed << std::setprecision(2) << pos.x << ", " 
              << pos.y << "), Rotation: " 
              << rotation << " rad" << std::endl;
}

// Test basic entity creation and component management
bool testEntityComponentBasics() {
    nf::World world;
    
    // Create entity
    auto entity = world.createEntity();
    assert(entity.isValid());
    assert(world.getEntityCount() == 1);
    
    // Add transform component
    auto& transform = entity.addComponent<nf::TransformComponent>(10.0f, 20.0f);
    assert(entity.hasComponent<nf::TransformComponent>());
    assert(approximatelyEqual(transform.getPosition().x, 10.0f));
    assert(approximatelyEqual(transform.getPosition().y, 20.0f));
    
    // Add velocity component
    auto& velocity = entity.addComponent<nf::VelocityComponent>(5.0f, 10.0f);
    assert(entity.hasComponent<nf::VelocityComponent>());
    assert(approximatelyEqual(velocity.getVelocity().x, 5.0f));
    assert(approximatelyEqual(velocity.getVelocity().y, 10.0f));
    
    // Test component retrieval
    auto& retrievedTransform = entity.getComponent<nf::TransformComponent>();
    assert(&retrievedTransform == &transform); // Same instance
    
    // Test component removal
    assert(entity.removeComponent<nf::VelocityComponent>());
    assert(!entity.hasComponent<nf::VelocityComponent>());
    
    // Test entity destruction
    assert(world.destroyEntity(entity));
    assert(world.getEntityCount() == 0);
    
    return true;
}

// Test basic linear movement
bool testLinearMovement() {
    nf::World world;
    
    // Add movement system
    auto& movementSystem = world.addSystem<nf::MovementSystem>();
    assert(world.getSystemCount() == 1);
    
    // Create entity with transform and velocity
    auto entity = world.createEntity();
    auto& transform = entity.addComponent<nf::TransformComponent>(0.0f, 0.0f);
    auto& velocity = entity.addComponent<nf::VelocityComponent>(10.0f, 20.0f);
    
    // Run simulation for 1 second
    float deltaTime = 0.1f;
    for (int i = 0; i < 10; ++i) {
        world.update(deltaTime);
    }
    
    // After 1 second, position should be (10, 20)
    assert(approximatelyEqual(transform.getPosition().x, 10.0f));
    assert(approximatelyEqual(transform.getPosition().y, 20.0f));
    
    return true;
}

// Test rotation movement
bool testRotationalMovement() {
    nf::World world;
    
    // Add movement system
    world.addSystem<nf::MovementSystem>();
    
    // Create entity with transform and velocity
    auto entity = world.createEntity();
    auto& transform = entity.addComponent<nf::TransformComponent>(0.0f, 0.0f);
    auto& velocity = entity.addComponent<nf::VelocityComponent>();
    
    // Set angular velocity (1 radian per second)
    velocity.setAngularVelocity(1.0f);
    
    // Run simulation for 1 second
    float deltaTime = 0.1f;
    for (int i = 0; i < 10; ++i) {
        world.update(deltaTime);
    }
    
    // After 1 second, rotation should be approximately 1 radian
    assert(approximatelyEqual(transform.getRotation(), 1.0f));
    
    return true;
}

// Test damping effects
bool testDamping() {
    nf::World world;
    
    // Add movement system with damping
    auto& movementSystem = world.addSystem<nf::MovementSystem>();
    movementSystem.setDamping(0.1f, 0.2f); // 10% linear, 20% angular damping per second
    
    // Create entity with transform and velocity
    auto entity = world.createEntity();
    auto& transform = entity.addComponent<nf::TransformComponent>();
    auto& velocity = entity.addComponent<nf::VelocityComponent>(10.0f, 0.0f);
    velocity.setAngularVelocity(10.0f);
    
    // Run simulation for 1 second
    float deltaTime = 0.1f;
    for (int i = 0; i < 10; ++i) {
        world.update(deltaTime);
    }
    
    // Verify damping effect on velocity
    // For linear: v' = v * (1 - 0.1*dt)^10
    float expectedLinearVelocity = 10.0f * std::pow(1.0f - 0.1f * deltaTime, 10);
    // For angular: w' = w * (1 - 0.2*dt)^10
    float expectedAngularVelocity = 10.0f * std::pow(1.0f - 0.2f * deltaTime, 10);
    
    assert(approximatelyEqual(velocity.getVelocity().x, expectedLinearVelocity, 0.01f));
    assert(approximatelyEqual(velocity.getAngularVelocity(), expectedAngularVelocity, 0.01f));
    
    return true;
}

// Test multiple entities with different movement patterns
bool testMultipleEntities() {
    nf::World world;
    
    // Add movement system
    world.addSystem<nf::MovementSystem>();
    
    // Create several entities with different movement patterns
    std::vector<nf::Entity> entities;
    
    // Entity 1: Linear motion
    auto entity1 = world.createEntity();
    entity1.addComponent<nf::TransformComponent>(0.0f, 0.0f);
    entity1.addComponent<nf::VelocityComponent>(10.0f, 0.0f);
    entities.push_back(entity1);
    
    // Entity 2: Circular motion setup
    auto entity2 = world.createEntity();
    entity2.addComponent<nf::TransformComponent>(0.0f, 0.0f);
    auto& vel2 = entity2.addComponent<nf::VelocityComponent>(0.0f, 10.0f);
    vel2.setAngularVelocity(1.0f);
    entities.push_back(entity2);
    
    // Entity 3: No velocity component - shouldn't move
    auto entity3 = world.createEntity();
    entity3.addComponent<nf::TransformComponent>(5.0f, 5.0f);
    entities.push_back(entity3);
    
    // Run simulation for 1 second
    float deltaTime = 0.1f;
    for (int i = 0; i < 10; ++i) {
        world.update(deltaTime);
    }
    
    // Verify entity 1 moved linearly
    const auto& transform1 = entity1.getComponent<nf::TransformComponent>();
    assert(approximatelyEqual(transform1.getPosition().x, 10.0f));
    assert(approximatelyEqual(transform1.getPosition().y, 0.0f));
    
    // Verify entity 3 didn't move (no velocity component)
    const auto& transform3 = entity3.getComponent<nf::TransformComponent>();
    assert(approximatelyEqual(transform3.getPosition().x, 5.0f));
    assert(approximatelyEqual(transform3.getPosition().y, 5.0f));
    
    return true;
}

// Test entity recycling
bool testEntityRecycling() {
    nf::World world;
    
    // Create and destroy a few entities
    auto entity1 = world.createEntity();
    auto entity2 = world.createEntity();
    
    // Destroy entity1
    world.destroyEntity(entity1);
    
    // Create new entity, should reuse ID but have different version
    auto entity3 = world.createEntity();
    
    // entity3 should be valid but different from entity1
    assert(entity3.isValid());
    assert(entity3.getID() == entity1.getID()); // Same ID
    assert(entity3.getVersion() != entity1.getVersion()); // Different version
    
    // original entity1 reference should be invalid
    assert(!world.isEntityValid(entity1));
    
    return true;
}

// Test component interaction (movement affecting transform)
bool testComponentInteraction() {
    nf::World world;
    
    // Add movement system
    world.addSystem<nf::MovementSystem>();
    
    // Create entity
    auto entity = world.createEntity();
    auto& transform = entity.addComponent<nf::TransformComponent>();
    auto& velocity = entity.addComponent<nf::VelocityComponent>(3.0f, 4.0f);
    
    // Run one update step
    world.update(1.0f);
    
    // Position should be updated based on velocity
    assert(approximatelyEqual(transform.getPosition().x, 3.0f));
    assert(approximatelyEqual(transform.getPosition().y, 4.0f));
    
    // Modify transform directly
    transform.setPosition(10.0f, 10.0f);
    
    // Run another update step
    world.update(1.0f);
    
    // Position should be correctly updated from the new position
    assert(approximatelyEqual(transform.getPosition().x, 13.0f));
    assert(approximatelyEqual(transform.getPosition().y, 14.0f));
    
    return true;
}

// Main test function
int main() {
    std::cout << "Starting NexusForge ECS Movement Tests" << std::endl;
    std::cout << "======================================" << std::endl;
    
    // Run all tests
    printTestResult("Entity and Component Basics", testEntityComponentBasics());
    printTestResult("Linear Movement", testLinearMovement());
    printTestResult("Rotational Movement", testRotationalMovement());
    printTestResult("Damping Effects", testDamping());
    printTestResult("Multiple Entities", testMultipleEntities());
    printTestResult("Entity Recycling", testEntityRecycling());
    printTestResult("Component Interaction", testComponentInteraction());
    
    std::cout << "======================================" << std::endl;
    std::cout << "Testing complete!" << std::endl;
    
    return 0;
}

