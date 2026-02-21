#pragma once

#include <M5Unified.h>
#include <M5GFX.h>
#include "PhysicsEngine.h"
#include "Sun.h"

/**
 * Particle structure for firework effects
 */
struct Particle {
    double x;              // X position (relative to center)
    double y;              // Y position (relative to center)
    double vx;             // X velocity
    double vy;             // Y velocity
    uint16_t color;        // Particle color
    int lifetime;          // Remaining lifetime (frames)
    int initialLifetime;   // Initial lifetime (for alpha calculation)
};

/**
 * Renderer Class
 * Handles rendering-related processes
 */
class Renderer {
public:
    /**
     * Constructor
     * @param display Display object
     */
    Renderer(M5GFX& display);

    /**
     * Initialize
     */
    void init();

    /**
     * Render planets and sun
     * @param physicsEngine Physics engine
     * @param isTouching Whether touch is active
     * @param touchStartX Touch start X coordinate
     * @param touchStartY Touch start Y coordinate
     */
    void render(const PhysicsEngine& physicsEngine, 
                bool isTouching, int touchStartX, int touchStartY);

    /**
     * Create firework effect at specified position
     * @param x X position (relative to center)
     * @param y Y position (relative to center)
     * @param color Color of the firework particles
     */
    void createFirework(double x, double y, uint16_t color);

    /**
     * Get X coordinate of screen center
     * @return X coordinate of screen center
     */
    int getCenterX() const;

    /**
     * Get Y coordinate of screen center
     * @return Y coordinate of screen center
     */
    int getCenterY() const;

    /**
     * Get maximum X coordinate for out-of-bounds detection
     * @return Maximum X coordinate for out-of-bounds detection
     */
    int getMaxBoundsX() const;

    /**
     * Get maximum Y coordinate for out-of-bounds detection
     * @return Maximum Y coordinate for out-of-bounds detection
     */
    int getMaxBoundsY() const;

private:
    M5GFX& display;  // Display object
    M5Canvas canvas;  // Canvas for drawing (initialized in constructor)
    Sun sun;  // Sun object
    int centerX, centerY;  // Center coordinates of display
    unsigned long lastDrawTime;  // Timer for drawing
    
    // Firework particles
    static constexpr int MAX_PARTICLES = FireworkConstants::MAX_EFFECTS * FireworkConstants::PARTICLE_COUNT;
    Particle particles[MAX_PARTICLES];
    int particleCount;

    /**
     * Alpha blend a color with black background
     * @param fg Foreground color
     * @param alpha Alpha value (0-255)
     * @return Blended color
     */
    uint16_t alphaBlend(uint16_t fg, uint8_t alpha) const;

    /**
     * Update firework particles
     */
    void updateParticles();

    /**
     * Draw firework particles
     */
    void drawParticles();

    /**
     * Draw an arrow
     * @param startX X coordinate of start point
     * @param startY Y coordinate of start point
     * @param endX X coordinate of end point
     * @param endY Y coordinate of end point
     * @param color Arrow color
     * @param headSize Size of arrow head
     */
    void drawArrow(int startX, int startY, int endX, int endY, uint16_t color, int headSize = 10);
};
