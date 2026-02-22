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
        
        // Trail transparency with non-linear gradient for smoother, more natural effect
        // Use exponential curve: alpha stays high longer, then fades faster at the end
        float normalizedPosition = 1.0f - (float)i / PlanetConstants::TRAIL_LENGTH;
        uint8_t alpha = 255 * pow(normalizedPosition, 2.0f);  // Quadratic falloff for beautiful fade
        
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
    // Method to generate cosmic vibrant colors
    // Creates colors reminiscent of nebulas and distant galaxies
    uint8_t r, g, b;
    
    // Select from multiple cosmic color palettes
    int palette = random(7);
    
    switch (palette) {
        case 0: // Magenta / Purple Nebula
            r = 180 + random(76);   // 180-255
            g = 50 + random(100);    // 50-149
            b = 200 + random(56);    // 200-255
            break;
        case 1: // Cyan / Blue Starlight
            r = 50 + random(80);     // 50-129
            g = 180 + random(76);    // 180-255
            b = 230 + random(26);    // 230-255
            break;
        case 2: // Golden / Warm Dwarf Star
            r = 230 + random(26);    // 230-255
            g = 160 + random(60);    // 160-219
            b = 80 + random(100);    // 80-179
            break;
        case 3: // Emerald / Teal Cosmic Dust
            r = 50 + random(100);    // 50-149
            g = 200 + random(56);    // 200-255
            b = 150 + random(70);    // 150-219
            break;
        case 4: // Rose / Pink Gas Cloud
            r = 230 + random(26);    // 230-255
            g = 100 + random(80);    // 100-179
            b = 180 + random(76);    // 180-255
            break;
        case 5: // Lavender / Purple Galaxy Core
            r = 160 + random(60);    // 160-219
            g = 120 + random(80);    // 120-199
            b = 220 + random(36);    // 220-255
            break;
        case 6: // Silver / White Dwarf
            r = 200 + random(56);    // 200-255
            g = 200 + random(56);    // 200-255
            b = 220 + random(36);    // 220-255
            break;
    }
    
    // Convert to RGB565 format
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
