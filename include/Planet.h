#pragma once

#include <M5Unified.h>
#include "Constants.h"

/**
 * Planet Class
 * Manages the physical properties and rendering information of planets
 */
class Planet {
public:
    /**
     * Constructor
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     * @param vx Initial X velocity
     * @param vy Initial Y velocity
     * @param color Planet color
     */
    Planet(double x, double y, double vx, double vy, uint16_t color);

    /**
     * Update planet position
     * @param ax Acceleration in X direction
     * @param ay Acceleration in Y direction
     * @param updateTrails Whether to update trail positions
     */
    void update(double ax, double ay, bool updateTrails);

    /**
     * Draw the planet
     * @param canvas Canvas to draw on
     * @param centerX X coordinate of screen center
     * @param centerY Y coordinate of screen center
     */
    void draw(M5Canvas& canvas, int centerX, int centerY) const;

    /**
     * Draw the trail
     * @param canvas Canvas to draw on
     * @param centerX X coordinate of screen center
     * @param centerY Y coordinate of screen center
     */
    void drawTrail(M5Canvas& canvas, int centerX, int centerY) const;

    /**
     * Determine if the planet is out of bounds
     * @param maxX Maximum X coordinate of the screen
     * @param maxY Maximum Y coordinate of the screen
     * @return true if out of bounds
     */
    bool isOutOfBounds(int maxX, int maxY) const;

    /**
     * Determine if the planet has collided with the sun
     * @return true if collided with the sun
     */
    bool isCollidedWithSun() const;

    // Getters for position and velocity
    double getX() const { return x; }
    double getY() const { return y; }
    double getVx() const { return vx; }
    double getVy() const { return vy; }
    uint16_t getColor() const { return color; }
    
    /**
     * Generate a random vibrant color
     * @return Generated color
     */
    static uint16_t randomPastelColor();

private:
    /**
     * Alpha blend function for colors (apply transparency)
     * @param fg Foreground color
     * @param bg Background color
     * @param alpha Alpha value (0-255)
     * @return Blended color
     */
    uint16_t alphaBlend(uint16_t fg, uint16_t bg, uint8_t alpha) const;

    double x, y;       // Position
    double vx, vy;     // Velocity
    uint16_t color;    // Color
    
    // Past positions (for trail effect) - using ring buffer
    int trailX[PlanetConstants::TRAIL_LENGTH];
    int trailY[PlanetConstants::TRAIL_LENGTH];
    int trailIndex;    // Ring buffer index for optimization
};
