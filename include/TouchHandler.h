#pragma once

#include <M5Unified.h>
#include "PhysicsEngine.h"
#include "Renderer.h"

/**
 * Touch Handler Class
 * Manages touch operations
 */
class TouchHandler {
public:
    /**
     * Constructor
     * @param physicsEngine Physics engine
     * @param renderer Renderer
     */
    TouchHandler(PhysicsEngine& physicsEngine, Renderer& renderer);

    /**
     * Process touch operations
     * @return true if touch is active
     */
    bool update();

    /**
     * Get X coordinate of touch start position
     * @return X coordinate of touch start position
     */
    int getTouchStartX() const;

    /**
     * Get Y coordinate of touch start position
     * @return Y coordinate of touch start position
     */
    int getTouchStartY() const;

private:
    PhysicsEngine& physicsEngine;  // Physics engine
    Renderer& renderer;  // Renderer
    bool isTouching;  // Whether touch is active
    int touchStartX;  // X coordinate of touch start position
    int touchStartY;  // Y coordinate of touch start position
};
