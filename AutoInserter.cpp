#include "AutoInserter.h"

AutoInserter::AutoInserter()
  : enabled(false),
    keyInterval(125),
    keyPressDuration(80),
    lastKeyTime(0),
    keyIsPressed(false),
    keyPressStartTime(0) {}

void AutoInserter::enable() {
  enabled = true;
  reset();
}

void AutoInserter::disable() {
  enabled = false;
  if (keyIsPressed) {
    Keyboard.release('2');
    keyIsPressed = false;
  }
}

void AutoInserter::toggle() {
  if (enabled) disable();
  else enable();
}

void AutoInserter::update() {
  if (!enabled) return;

  unsigned long now = millis();

  if (!keyIsPressed && (now - lastKeyTime >= keyInterval)) {
    Keyboard.press('2');
    keyIsPressed = true;
    keyPressStartTime = now;
  }

  if (keyIsPressed && (now - keyPressStartTime >= keyPressDuration)) {
    Keyboard.release('2');
    keyIsPressed = false;
    lastKeyTime = now;
  }
}

bool AutoInserter::isEnabled() const {
  return enabled;
}

void AutoInserter::reset() {
  lastKeyTime = millis();
  keyIsPressed = false;
}
