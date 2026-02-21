#include <M5Unified.h>
#include "Constants.h"
#include "PhysicsEngine.h"
#include "Renderer.h"
#include "TouchHandler.h"
#include "Sun.h"

// Global variables
Renderer renderer(M5.Display);
PhysicsEngine physicsEngine(renderer);
TouchHandler touchHandler(physicsEngine, renderer);

void setup() {
  // Initialize M5 device
  auto cfg = M5.config();
  M5.begin(cfg);

  // Set volume
  M5.Speaker.setVolume(ToneConstants::SPEAKER_VOLUME);

  // Play boot sound
  M5.Speaker.tone(ToneConstants::TOUCH_TONE_FREQUENCY, ToneConstants::TONE_DURATION);
  delay(100);
  M5.Speaker.tone(ToneConstants::TOUCH_TONE_FREQUENCY, ToneConstants::TONE_DURATION);
  delay(100);
  M5.Speaker.tone(ToneConstants::TOUCH_TONE_FREQUENCY, ToneConstants::TONE_DURATION);

  // Initialize random seed
  randomSeed(millis());
  
  // Initialize renderer
  renderer.init();
}

void loop() {
  M5.update();  // Update button states
  
  // Process touch operations
  bool isTouching = touchHandler.update();
  
  // Update physics simulation
  physicsEngine.update();
  
  // Remove planets that are out of bounds
  physicsEngine.removeOutOfBoundsPlanets(
    renderer.getMaxBoundsX(), 
    renderer.getMaxBoundsY()
  );
  
  // Render
  renderer.render(
    physicsEngine, 
    isTouching, 
    touchHandler.getTouchStartX(), 
    touchHandler.getTouchStartY()
  );
}
