#include "PhysicsEngine.h"
#include "Renderer.h"
#include <cmath>

PhysicsEngine::PhysicsEngine(Renderer& renderer) 
    : renderer(renderer),
      lastTrailUpdateTime(0),
      distanceScaleSquared(PhysicsConstants::DISTANCE_SCALE * PhysicsConstants::DISTANCE_SCALE),
      collisionEffectActive(false),
      collisionEffectX(0),
      collisionEffectY(0),
      collisionEffectStartTime(0) {
    // Reserve space for maximum planets to avoid reallocation
    planets.reserve(PlanetConstants::MAX_COUNT);
    accelerationX.reserve(PlanetConstants::MAX_COUNT);
    accelerationY.reserve(PlanetConstants::MAX_COUNT);
}

void PhysicsEngine::addPlanet(float x, float y, float vx, float vy, uint16_t color) {
    planets.emplace_back(x, y, vx, vy, color);
    if (planets.size() > PlanetConstants::MAX_COUNT) {
        planets.erase(planets.begin());
    }
}

bool PhysicsEngine::shouldUpdateTrails() {
    unsigned long currentTime = millis();
    if (currentTime - lastTrailUpdateTime > RenderConstants::TRAIL_UPDATE_INTERVAL) {
        lastTrailUpdateTime = currentTime;
        return true;
    }
    return false;
}

void PhysicsEngine::calculateSunGravity(size_t planetIndex, std::vector<float>& ax, std::vector<float>& ay) {
    // Calculate gravity from the sun (sun is at origin (0,0))
    float dx = -planets[planetIndex].getX();
    float dy = -planets[planetIndex].getY();
    float r2 = dx*dx + dy*dy;
    
    // Optimized calculation using inverse square root
    // F = G * m1 * m2 / r^2, a = F / m
    // Combined: a = G * m2 * dx / (r^3) where r^3 = r * r^2
    float r = sqrtf(r2);
    float r_inv3 = 1.0f / (r * r2);  // 1/r^3 using only one sqrt
    
    // Acceleration calculation (optimized to reduce operations)
    float factor = PhysicsConstants::G * SunConstants::MASS * r_inv3 / distanceScaleSquared;
    float accelX = factor * dx;
    float accelY = factor * dy;
    
    ax[planetIndex] += accelX;
    ay[planetIndex] += accelY;
}

void PhysicsEngine::calculatePlanetGravity(std::vector<float>& ax, std::vector<float>& ay) {
    // Calculate gravity between planets (skip calculation for distant planets to reduce processing load)
    for (size_t i = 0; i < planets.size(); i++) {
        for (size_t j = i + 1; j < planets.size(); j++) {
            // Calculate distance between planets
            float dx = planets[j].getX() - planets[i].getX();
            float dy = planets[j].getY() - planets[i].getY();
            float r2 = dx*dx + dy*dy;
            
            // Skip calculation if distance is too far (to reduce processing load)
            if (r2 > PhysicsConstants::MAX_FORCE_DISTANCE_SQUARED) {
                continue;
            }
            
            // Apply minimum distance (to prevent collision)
            if (r2 < PhysicsConstants::MIN_DISTANCE_SQUARED) {
                r2 = PhysicsConstants::MIN_DISTANCE_SQUARED;
            }
            
            // Optimized calculation using inverse cube root
            // a = G * m * dx / (r^3) where r^3 = r * r^2
            float r = sqrtf(r2);
            float r_inv3 = 1.0f / (r * r2);  // 1/r^3 using only one sqrt
            
            // Acceleration calculation (optimized)
            float factor = PhysicsConstants::G * PlanetConstants::MASS * r_inv3 / distanceScaleSquared;
            float accelX = factor * dx;
            float accelY = factor * dy;
            
            // Acceleration for planet i
            ax[i] += accelX;
            ay[i] += accelY;
            
            // Acceleration for planet j (opposite direction)
            ax[j] -= accelX;
            ay[j] -= accelY;
        }
    }
}

bool PhysicsEngine::update() {
    // Determine if trail positions need to be updated
    bool shouldUpdateTrailPositions = shouldUpdateTrails();
    
    // Check if collision effect has expired
    if (collisionEffectActive) {
        unsigned long currentTime = millis();
        if (currentTime - collisionEffectStartTime > CollisionConstants::COLLISION_EFFECT_DURATION) {
            collisionEffectActive = false;
        }
    }
    
    // Early return if there are no planets
    if (planets.empty()) {
        return shouldUpdateTrailPositions;
    }
    
    // Reuse acceleration arrays (resize and clear)
    accelerationX.resize(planets.size());
    accelerationY.resize(planets.size());
    std::fill(accelerationX.begin(), accelerationX.end(), 0.0f);
    std::fill(accelerationY.begin(), accelerationY.end(), 0.0f);
    
    // Apply gravity from the sun to each planet
    for (size_t i = 0; i < planets.size(); i++) {
        calculateSunGravity(i, accelerationX, accelerationY);
    }
    
    // Calculate gravity between planets
    calculatePlanetGravity(accelerationX, accelerationY);
    
    // Update velocity and position of each planet
    for (size_t i = 0; i < planets.size(); i++) {
        planets[i].update(accelerationX[i], accelerationY[i], shouldUpdateTrailPositions);
    }
    
    return shouldUpdateTrailPositions;
}

void PhysicsEngine::removeOutOfBoundsPlanets(int maxX, int maxY) {
    // Early return if there are no planets
    if (planets.empty()) {
        return;
    }
    
    // Remove planets that are out of bounds or have collided with the sun
    // Process from the end to prevent index shifting due to removal
    for (int i = static_cast<int>(planets.size()) - 1; i >= 0; i--) {
        const Planet& planet = planets[i];
        
        // Remove planets that are out of bounds
        if (planet.isOutOfBounds(maxX, maxY)) {
            planets.erase(planets.begin() + i);
        } 
        // Remove planets that have collided with the sun and play sound effect
        else if (planet.isCollidedWithSun()) {
            // Record collision position for effect
            collisionEffectActive = true;
            collisionEffectX = planet.getX();
            collisionEffectY = planet.getY();
            collisionEffectStartTime = millis();
            
            // Create firework effect at collision position
            renderer.createFirework(collisionEffectX, collisionEffectY, planet.getColor());
            
            // Play sound effect
            M5.Speaker.tone(ToneConstants::COLLISION_TONE_FREQUENCY, ToneConstants::TONE_DURATION);
            
            // Remove the planet
            planets.erase(planets.begin() + i);
        }
    }
}

size_t PhysicsEngine::getPlanetCount() const {
    return planets.size();
}

const std::vector<Planet>& PhysicsEngine::getPlanets() const {
    return planets;
}

bool PhysicsEngine::hasActiveCollisionEffect() const {
    return collisionEffectActive;
}

float PhysicsEngine::getCollisionEffectX() const {
    return collisionEffectX;
}

float PhysicsEngine::getCollisionEffectY() const {
    return collisionEffectY;
}
