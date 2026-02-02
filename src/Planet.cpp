#include "Planet.h"
#include <cmath>

uint16_t Planet::alphaBlend(uint16_t fg, uint16_t bg, uint8_t alpha) const {
    // Decompose color into RGB components
    uint8_t fgR = (fg >> 11) & 0x1F;
    uint8_t fgG = (fg >> 5) & 0x3F;
    uint8_t fgB = fg & 0x1F;
    
    uint8_t bgR = (bg >> 11) & 0x1F;
    uint8_t bgG = (bg >> 5) & 0x3F;
    uint8_t bgB = bg & 0x1F;
    
    // Alpha blend (linear interpolation)
    uint8_t r = (fgR * alpha + bgR * (255 - alpha)) / 255;
    uint8_t g = (fgG * alpha + bgG * (255 - alpha)) / 255;
    uint8_t b = (fgB * alpha + bgB * (255 - alpha)) / 255;
    
    // Convert back to RGB565 format
    return (r << 11) | (g << 5) | b;
}

Planet::Planet(double x, double y, double vx, double vy, uint16_t color)
    : x(x), y(y), vx(vx), vy(vy), color(color), trailIndex(0) {
    // Initialize trail positions
    for (int i = 0; i < PlanetConstants::TRAIL_LENGTH; i++) {
        trailX[i] = x;
        trailY[i] = y;
    }
}

void Planet::update(double ax, double ay, bool updateTrails) {
    // Update velocity: v = v + a * dt
    vx += ax * PhysicsConstants::TIME_SCALE;
    vy += ay * PhysicsConstants::TIME_SCALE;
    
    // Update position: p = p + v * dt
    x += vx * PhysicsConstants::TIME_SCALE;
    y += vy * PhysicsConstants::TIME_SCALE;
    
    // Update trail positions using ring buffer (optimized - O(1) instead of O(n))
    if (updateTrails) {
        trailIndex = (trailIndex + 1) % PlanetConstants::TRAIL_LENGTH;
        trailX[trailIndex] = x;
        trailY[trailIndex] = y;
    }
}

void Planet::draw(M5Canvas& canvas, int centerX, int centerY) const {
    // Draw the planet body
    int screenX = centerX + x;
    int screenY = centerY + y;
    canvas.fillCircle(screenX, screenY, PlanetConstants::RADIUS, color);
}

void Planet::drawTrail(M5Canvas& canvas, int centerX, int centerY) const {
    // Draw trails using ring buffer (newest to oldest)
    for (int i = 0; i < PlanetConstants::TRAIL_LENGTH; i++) {
        // Calculate ring buffer index (from newest to oldest)
        int idx = (trailIndex - i + PlanetConstants::TRAIL_LENGTH) % PlanetConstants::TRAIL_LENGTH;
        
        // Calculate screen position of trail point
        int trailScreenX = centerX + trailX[idx];
        int trailScreenY = centerY + trailY[idx];
        
        // Trail transparency (older points are more transparent)
        uint8_t alpha = 255 * (PlanetConstants::TRAIL_LENGTH - i) / PlanetConstants::TRAIL_LENGTH;
        
        // Trail color (faded version of original color)
        uint16_t trailColor = alphaBlend(color, BLACK, alpha);
        
        // Draw trail
        canvas.drawPixel(trailScreenX, trailScreenY, trailColor);
    }
}

bool Planet::isOutOfBounds(int maxX, int maxY) const {
    return (abs(x) > maxX || abs(y) > maxY);
}

bool Planet::isCollidedWithSun() const {
    // Collision detected if distance from sun center is less than sun radius
    double distanceSquared = x*x + y*y;
    return (distanceSquared < SunConstants::RADIUS_SQUARED);
}

uint16_t Planet::randomPastelColor() {
    // Method to generate vibrant colors
    uint8_t r, g, b;
    
    // Randomly select one of three components to be high, others to be low-medium
    switch (random(3)) {
        case 0: // Emphasize red
            r = 200 + random(56);  // 200-255
            g = 80 + random(150);  // 80-229
            b = 80 + random(150);  // 80-229
            break;
        case 1: // Emphasize green
            r = 80 + random(150);  // 80-229
            g = 200 + random(56);  // 200-255
            b = 80 + random(150);  // 80-229
            break;
        case 2: // Emphasize blue
            r = 80 + random(150);  // 80-229
            g = 80 + random(150);  // 80-229
            b = 200 + random(56);  // 200-255
            break;
    }
    
    // Convert to RGB565 format
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
