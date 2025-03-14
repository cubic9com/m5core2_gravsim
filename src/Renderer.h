#pragma once

#include <M5Unified.h>
#include <M5GFX.h>
#include "PhysicsEngine.h"
#include "Sun.h"

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
