#pragma once

#include <M5Unified.h>
#include <cstdint>

// Tone settings for feedback sound
namespace ToneConstants {
    const uint8_t SPEAKER_VOLUME = 48U;
    const float TOUCH_TONE_FREQUENCY = 659.26F;
    const float COLLISION_TONE_FREQUENCY = 82.407F;
    const uint32_t TONE_DURATION = 50U;
}

// Constants related to physics simulation
namespace PhysicsConstants {
    // Gravitational constant
    constexpr double G = 6.67430e-11;
    // Time scale (to speed up simulation)
    constexpr double TIME_SCALE = 2.0e10;
    // Distance scale (to adjust display)
    constexpr double DISTANCE_SCALE = 1.0e9;
    // Minimum distance (to prevent collision)
    constexpr double MIN_DISTANCE = 3.0;
    // Skip gravity calculation between planets beyond this distance (to reduce processing load)
    constexpr double MAX_FORCE_DISTANCE = 100.0;
    // Velocity factor
    constexpr double SPEED_FACTOR = 2.0e-14;
}

// Constants related to the sun
namespace SunConstants {
    // Sun's mass
    constexpr double MASS = 3.33e5;
    // Sun's radius (pixels)
    constexpr int RADIUS = 10;
    // Sun's base color
    constexpr uint16_t BASE_COLOR = TFT_ORANGE;
    // Intensity of sun's brightness fluctuation
    constexpr float BRIGHTNESS_FLUCTUATION = 0.1f;  // Brightness fluctuation range (0-1)
}

// Constants related to planets
namespace PlanetConstants {
    // Planet's mass
    constexpr double MASS = 30000;
    // Planet's radius (pixels)
    constexpr int RADIUS = 2;
    // Maximum number of planets
    constexpr int MAX_COUNT = 10;
    // Number of trail points
    constexpr int TRAIL_LENGTH = 10;
}

// Constants related to rendering
namespace RenderConstants {
    // Drawing update interval (milliseconds)
    constexpr unsigned long DRAW_INTERVAL = 70;
    // Trail update interval (milliseconds)
    constexpr unsigned long TRAIL_UPDATE_INTERVAL = 35;
    // Margin to determine off-screen (pixels)
    constexpr int SCREEN_MARGIN = 20;
}

// Constants related to collision effects
namespace CollisionConstants {
    // Radius of collision effect (pixels)
    constexpr int COLLISION_EFFECT_RADIUS = 4;
    // Color of collision effect
    constexpr uint16_t COLLISION_EFFECT_COLOR = TFT_YELLOW;
    // Duration of collision effect (milliseconds)
    constexpr unsigned long COLLISION_EFFECT_DURATION = 105;
}
