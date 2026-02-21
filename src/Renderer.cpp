#include "Renderer.h"
#include "Constants.h"
#include <cmath>

Renderer::Renderer(M5GFX& display) 
    : display(display), canvas(&display), sun(), lastDrawTime(0), particleCount(0) {
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
    
    // Draw firework particles
    drawParticles();
    
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
    
    // Update particles after rendering
    updateParticles();
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

void Renderer::createFirework(double x, double y, uint16_t color) {
    // Create particles for firework effect
    for (int i = 0; i < FireworkConstants::PARTICLE_COUNT; i++) {
        if (particleCount >= MAX_PARTICLES) {
            break;  // Maximum particles reached
        }
        
        // Random angle (0-360 degrees)
        float angle = random(360) * DEG_TO_RAD;
        
        // Random speed variation (0.7x to 1.3x of base speed)
        float speed = FireworkConstants::PARTICLE_SPEED * (0.7f + random(60) / 100.0f);
        
        // Set particle properties
        particles[particleCount].x = x;
        particles[particleCount].y = y;
        particles[particleCount].vx = speed * cos(angle);
        particles[particleCount].vy = speed * sin(angle);
        particles[particleCount].color = color;
        particles[particleCount].lifetime = FireworkConstants::PARTICLE_LIFETIME;
        particles[particleCount].initialLifetime = FireworkConstants::PARTICLE_LIFETIME;
        
        particleCount++;
    }
}

void Renderer::updateParticles() {
    // Update all particles
    for (int i = particleCount - 1; i >= 0; i--) {
        // Update position
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        
        // Decrease lifetime
        particles[i].lifetime--;
        
        // Remove dead particles
        if (particles[i].lifetime <= 0) {
            // Move last particle to current position (to maintain compact array)
            particles[i] = particles[particleCount - 1];
            particleCount--;
        }
    }
}

uint16_t Renderer::alphaBlend(uint16_t fg, uint8_t alpha) const {
    // Decompose color into RGB components
    uint8_t fgR = (fg >> 11) & 0x1F;
    uint8_t fgG = (fg >> 5) & 0x3F;
    uint8_t fgB = fg & 0x1F;
    
    // Alpha blend with black background
    uint8_t r = (fgR * alpha) / 255;
    uint8_t g = (fgG * alpha) / 255;
    uint8_t b = (fgB * alpha) / 255;
    
    // Convert back to RGB565 format
    return (r << 11) | (g << 5) | b;
}

void Renderer::drawParticles() {
    for (int i = 0; i < particleCount; i++) {
        // Calculate alpha based on remaining lifetime
        uint8_t alpha = 255 * particles[i].lifetime / particles[i].initialLifetime;
        
        // Calculate particle size (shrinks as it fades)
        int radius = 2 * alpha / 255;
        if (radius < 1) radius = 1;
        
        // Calculate screen position
        int screenX = centerX + static_cast<int>(particles[i].x);
        int screenY = centerY + static_cast<int>(particles[i].y);
        
        // Draw particle with alpha blending
        uint16_t blendedColor = alphaBlend(particles[i].color, alpha);
        canvas.fillCircle(screenX, screenY, radius, blendedColor);
    }
}
