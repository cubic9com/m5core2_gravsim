#pragma once

#include <vector>
#include "Planet.h"
#include "Constants.h"

/**
 * Physics Engine Class
 * Calculates planet movements and performs physics simulation
 */
class PhysicsEngine {
public:
    /**
     * Constructor
     */
    PhysicsEngine();

    /**
     * Add a planet
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     * @param vx Initial X velocity
     * @param vy Initial Y velocity
     * @param color Planet color
     */
    void addPlanet(double x, double y, double vx, double vy);

    /**
     * Update physics simulation
     * @return Whether trail positions were updated
     */
    bool update();

    /**
     * Remove planets that are out of bounds
     * @param maxX Maximum X coordinate of the screen
     * @param maxY Maximum Y coordinate of the screen
     */
    void removeOutOfBoundsPlanets(int maxX, int maxY);

    /**
     * Get the number of planets
     * @return Number of planets
     */
    size_t getPlanetCount() const;

    /**
     * Get the collection of planets
     * @return Collection of planets
     */
    const std::vector<Planet>& getPlanets() const;
    
    /**
     * Check if there is an active collision effect
     * @return true if there is an active collision effect
     */
    bool hasActiveCollisionEffect() const;
    
    /**
     * Get the X coordinate of the collision effect
     * @return X coordinate of the collision effect
     */
    double getCollisionEffectX() const;
    
    /**
     * Get the Y coordinate of the collision effect
     * @return Y coordinate of the collision effect
     */
    double getCollisionEffectY() const;

private:
    /**
     * Calculate gravity from the sun
     * @param planetIndex Index of the planet
     * @param ax Array of X components of acceleration
     * @param ay Array of Y components of acceleration
     */
    void calculateSunGravity(size_t planetIndex, std::vector<double>& ax, std::vector<double>& ay);

    /**
     * Calculate gravity between planets
     * @param ax Array of X components of acceleration
     * @param ay Array of Y components of acceleration
     */
    void calculatePlanetGravity(std::vector<double>& ax, std::vector<double>& ay);

    /**
     * Determine if trail positions need to be updated
     * @return true if trails need to be updated
     */
    bool shouldUpdateTrails();

    std::vector<Planet> planets;  // Collection of planets
    unsigned long lastTrailUpdateTime;  // Timer for trail updates
    const double distanceScaleSquared;  // Square of distance scale (for optimization)
    
    // Collision effect variables
    bool collisionEffectActive;  // Whether there is an active collision effect
    double collisionEffectX;     // X coordinate of the collision effect
    double collisionEffectY;     // Y coordinate of the collision effect
    unsigned long collisionEffectStartTime;  // Start time of the collision effect
    
    // Reusable acceleration arrays (for optimization)
    std::vector<double> accelerationX;
    std::vector<double> accelerationY;
};
