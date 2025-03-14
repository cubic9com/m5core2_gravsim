#include "PhysicsEngine.h"
#include <cmath>

PhysicsEngine::PhysicsEngine() 
    : lastTrailUpdateTime(0),
      distanceScaleSquared(PhysicsConstants::DISTANCE_SCALE * PhysicsConstants::DISTANCE_SCALE),
      collisionEffectActive(false),
      collisionEffectX(0),
      collisionEffectY(0),
      collisionEffectStartTime(0) {
}

void PhysicsEngine::addPlanet(double x, double y, double vx, double vy) {
    uint16_t color = Planet::randomPastelColor();
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

void PhysicsEngine::calculateSunGravity(size_t planetIndex, std::vector<double>& ax, std::vector<double>& ay) {
    // Calculate gravity from the sun (sun is at origin (0,0))
    double dx = -planets[planetIndex].getX();
    double dy = -planets[planetIndex].getY();
    double r2 = dx*dx + dy*dy;
    double r = sqrt(r2);
    
    // Law of universal gravitation: F = G * m1 * m2 / r^2
    double force = PhysicsConstants::G * PlanetConstants::MASS * SunConstants::MASS / (r * r * distanceScaleSquared);
    
    // Acceleration: a = F / m
    double accelX = force * dx / (r * PlanetConstants::MASS);
    double accelY = force * dy / (r * PlanetConstants::MASS);
    
    ax[planetIndex] += accelX;
    ay[planetIndex] += accelY;
}

void PhysicsEngine::calculatePlanetGravity(std::vector<double>& ax, std::vector<double>& ay) {
    // Calculate gravity between planets (skip calculation for distant planets to reduce processing load)
    for (size_t i = 0; i < planets.size(); i++) {
        for (size_t j = i + 1; j < planets.size(); j++) {
            // Calculate distance between planets
            double dx = planets[j].getX() - planets[i].getX();
            double dy = planets[j].getY() - planets[i].getY();
            double r2 = dx*dx + dy*dy;
            
            // Skip calculation if distance is too far (to reduce processing load)
            const double maxDistanceSquared = PhysicsConstants::MAX_FORCE_DISTANCE * PhysicsConstants::MAX_FORCE_DISTANCE;
            if (r2 > maxDistanceSquared) {
                continue;
            }
            
            // Apply minimum distance (to prevent collision)
            const double minDistanceSquared = PhysicsConstants::MIN_DISTANCE * PhysicsConstants::MIN_DISTANCE;
            if (r2 < minDistanceSquared) {
                r2 = minDistanceSquared;
            }
            
            double r = sqrt(r2);
            
            // Law of universal gravitation: F = G * m1 * m2 / r^2
            double force = PhysicsConstants::G * PlanetConstants::MASS * PlanetConstants::MASS / (r * r * distanceScaleSquared);
            
            // Acceleration: a = F / m
            double accelX = force * dx / (r * PlanetConstants::MASS);
            double accelY = force * dy / (r * PlanetConstants::MASS);
            
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
    
    // Initialize acceleration for each planet
    std::vector<double> accelerationX(planets.size(), 0.0);
    std::vector<double> accelerationY(planets.size(), 0.0);
    
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

double PhysicsEngine::getCollisionEffectX() const {
    return collisionEffectX;
}

double PhysicsEngine::getCollisionEffectY() const {
    return collisionEffectY;
}
