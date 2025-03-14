#include "Renderer.h"
#include "Constants.h"

Renderer::Renderer(M5GFX& display) 
    : display(display), canvas(&display), sun(), lastDrawTime(0) {
}

void Renderer::init() {
    // Get center coordinates of display
    centerX = display.width() / 2;
    centerY = display.height() / 2;
    
    // Initialize canvas
    canvas.createSprite(display.width(), display.height());
    canvas.setColorDepth(16);  // 16-bit color
}

void Renderer::render(const PhysicsEngine& physicsEngine, 
                      bool isTouching, int touchStartX, int touchStartY) {
    // Redraw at regular intervals (wider intervals to reduce processing load)
    unsigned long currentTime = millis();
    if (currentTime - lastDrawTime <= RenderConstants::DRAW_INTERVAL) {
        return;  // Skip if drawing interval is too short
    }
    lastDrawTime = currentTime;
    
    // Clear canvas
    canvas.fillScreen(BLACK);
    
    // Draw collision effect if active
    if (physicsEngine.hasActiveCollisionEffect()) {
        int effectX = centerX + physicsEngine.getCollisionEffectX();
        int effectY = centerY + physicsEngine.getCollisionEffectY();
        canvas.fillCircle(effectX, effectY, CollisionConstants::COLLISION_EFFECT_RADIUS, CollisionConstants::COLLISION_EFFECT_COLOR);
    }
    
    // Draw the sun
    sun.draw(canvas, centerX, centerY);
    
    // First draw trails for all planets
    const auto& planets = physicsEngine.getPlanets();
    for (const auto& planet : planets) {
        planet.drawTrail(canvas, centerX, centerY);
    }
    
    // Then draw all planet bodies (overlaid on trails)
    for (const auto& planet : planets) {
        planet.draw(canvas, centerX, centerY);
    }
    
    // If touching, draw drag arrow
    if (isTouching) {
        // Draw small circle at touch start position
        canvas.drawCircle(touchStartX, touchStartY, PlanetConstants::RADIUS, TFT_WHITE);
        
        // Draw arrow from touch start position to current position
        drawArrow(touchStartX, touchStartY, 
                  M5.Touch.getDetail().x, M5.Touch.getDetail().y, 
                  TFT_WHITE);
    }
    
    // Display number of planets
    canvas.setCursor(10, 10);
    canvas.printf("Planets: %d", physicsEngine.getPlanetCount());
    
    // Transfer canvas content to display
    canvas.pushSprite(0, 0);
}

int Renderer::getCenterX() const {
    return centerX;
}

int Renderer::getCenterY() const {
    return centerY;
}

int Renderer::getMaxBoundsX() const {
    return display.width() / 2 + RenderConstants::SCREEN_MARGIN;
}

int Renderer::getMaxBoundsY() const {
    return display.height() / 2 + RenderConstants::SCREEN_MARGIN;
}

void Renderer::drawArrow(int startX, int startY, int endX, int endY, uint16_t color, int headSize) {
    // Draw line
    canvas.drawLine(startX, startY, endX, endY, color);
    
    // Calculate direction vector of arrow
    float dx = endX - startX;
    float dy = endY - startY;
    
    // Calculate vector length
    float length = sqrt(dx * dx + dy * dy);
    
    // Don't draw if length is 0
    if (length < 1) return;
    
    // Normalize direction vector
    dx /= length;
    dy /= length;
    
    // Calculate position of arrow tip
    int arrowX = endX;
    int arrowY = endY;
    
    // Calculate vertices of triangle for arrow tip
    // Calculate vector perpendicular to direction vector (90 degree rotation)
    float perpX = -dy;
    float perpY = dx;
    
    // Three vertices of triangle
    int x1 = arrowX;
    int y1 = arrowY;
    int x2 = arrowX - headSize * dx + headSize * 0.5 * perpX;
    int y2 = arrowY - headSize * dy + headSize * 0.5 * perpY;
    int x3 = arrowX - headSize * dx - headSize * 0.5 * perpX;
    int y3 = arrowY - headSize * dy - headSize * 0.5 * perpY;
    
    // Draw filled triangle
    canvas.fillTriangle(x1, y1, x2, y2, x3, y3, color);
}
