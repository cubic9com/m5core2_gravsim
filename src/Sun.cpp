#include "Sun.h"
#include <cmath>

Sun::Sun() {
}

void Sun::draw(M5Canvas& canvas, int centerX, int centerY) {
    // Calculate sun color
    uint16_t sunColor = calculateColor();
    
    // Draw the sun
    canvas.fillCircle(centerX, centerY, SunConstants::RADIUS, sunColor);
    
    // Draw rays from the sun
    for (int i = 0; i < 10; i++) {
        // Random angle (0-359 degrees)
        float angle = random(360) * DEG_TO_RAD;
        
        // Line start point (sun center)
        int startX = centerX;
        int startY = centerY;
        
        // Line end point (15 pixels from sun center, at random angle)
        int length = 10 + random(3);
        int endX = centerX + length * cos(angle);
        int endY = centerY + length * sin(angle);
        
        // Draw line (same color as sun)
        canvas.drawLine(startX, startY, endX, endY, sunColor);
    }
}

uint16_t Sun::calculateColor() const {
    // Get YELLOW color components (RGB565 format)
    uint8_t baseR = (SunConstants::BASE_COLOR >> 11) & 0x1F;
    uint8_t baseG = (SunConstants::BASE_COLOR >> 5) & 0x3F;
    uint8_t baseB = SunConstants::BASE_COLOR & 0x1F;
    
    // Generate random brightness fluctuation (range 0.7-1.3)
    float brightness = 1.0f + (random(-100, 101) / 100.0f) * SunConstants::BRIGHTNESS_FLUCTUATION;
    
    // Apply brightness to color components (ensuring they stay within range)
    uint8_t r = constrain(baseR * brightness, 0, 31);
    uint8_t g = constrain(baseG * brightness, 0, 63);
    uint8_t b = constrain(baseB * brightness, 0, 31);
    
    // Convert back to RGB565 format
    return (r << 11) | (g << 5) | b;
}

double Sun::getMass() const {
    return SunConstants::MASS;
}

int Sun::getRadius() const {
    return SunConstants::RADIUS;
}
