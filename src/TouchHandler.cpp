#include "TouchHandler.h"
#include "Constants.h"
#include "Planet.h"

TouchHandler::TouchHandler(PhysicsEngine& physicsEngine, Renderer& renderer)
    : physicsEngine(physicsEngine), renderer(renderer),
      isTouching(false), touchStartX(0), touchStartY(0) {
}

bool TouchHandler::update() {
    // Process touch operations
    if (M5.Touch.getCount() > 0) {
        auto touch = M5.Touch.getDetail();
        
        // When touch begins
        if (!isTouching && touch.isPressed()) {
            isTouching = true;
            touchStartX = touch.x;
            touchStartY = touch.y;
            // Play a tone as feedback
            M5.Speaker.tone(ToneConstants::TOUCH_TONE_FREQUENCY, ToneConstants::TONE_DURATION);
        }
    } else if (isTouching) {
        // When touch ends
        auto touch = M5.Touch.getDetail();
        
        // Calculate initial velocity from drag distance and direction
        double dx = touch.x - touchStartX;
        double dy = touch.y - touchStartY;
        
        // Velocity magnitude is proportional to distance
        double vx = dx * PhysicsConstants::SPEED_FACTOR;
        double vy = dy * PhysicsConstants::SPEED_FACTOR;
        
        // Convert planet position to sun-centered coordinate system
        double planetX = touchStartX - renderer.getCenterX();
        double planetY = touchStartY - renderer.getCenterY();
        
        // Generate color for both planet and ripple
        uint16_t planetColor = Planet::randomPastelColor();
        
        // Create ripple effect at planet creation position
        renderer.createRipple(planetX, planetY, planetColor);
        
        // Add new planet with the same color
        physicsEngine.addPlanet(planetX, planetY, vx, vy, planetColor);
        
        // Reset touch state
        isTouching = false;
    }
    
    return isTouching;
}

int TouchHandler::getTouchStartX() const {
    return touchStartX;
}

int TouchHandler::getTouchStartY() const {
    return touchStartY;
}
