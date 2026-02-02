#pragma once

#include <M5Unified.h>
#include <M5GFX.h>
#include "Constants.h"

/**
 * Sun Class
 * Manages the rendering and physical properties of the sun
 */
class Sun {
public:
    /**
     * Constructor
     */
    Sun();

    /**
     * Draw the sun
     * @param canvas Canvas to draw on
     * @param centerX X coordinate of screen center
     * @param centerY Y coordinate of screen center
     */
    void draw(M5Canvas& canvas, int centerX, int centerY);

    /**
     * Get the sun's mass
     * @return Sun's mass
     */
    double getMass() const;

    /**
     * Get the sun's radius
     * @return Sun's radius
     */
    int getRadius() const;

private:
    /**
     * Calculate the sun's color
     * @return Sun's color
     */
    uint16_t calculateColor();
    
    // Cached color for optimization
    uint16_t cachedColor;
    unsigned long lastColorUpdateTime;
    static constexpr unsigned long COLOR_UPDATE_INTERVAL = 100; // milliseconds
};
