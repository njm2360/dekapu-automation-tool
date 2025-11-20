#include "AutoClicker.h"

AutoClicker::AutoClicker()
  : enabled(false),
    clickInterval(100),
    mousePressDuration(50),
    lastClickTime(0),
    mouseIsPressed(false),
    mousePressStartTime(0) {}

void AutoClicker::enable() {
  enabled = true;
  reset();
}

void AutoClicker::disable() {
  enabled = false;
  if (mouseIsPressed) {
    Mouse.release(MOUSE_LEFT);
    mouseIsPressed = false;
  }
}

void AutoClicker::toggle() {
  if (enabled) disable();
  else enable();
}

void AutoClicker::update() {
  if (!enabled) return;

  unsigned long now = millis();

  if (!mouseIsPressed && (now - lastClickTime >= clickInterval)) {
    Mouse.press(MOUSE_LEFT);
    mouseIsPressed = true;
    mousePressStartTime = now;
  }

  if (mouseIsPressed && (now - mousePressStartTime >= mousePressDuration)) {
    Mouse.release(MOUSE_LEFT);
    mouseIsPressed = false;
    lastClickTime = now;
  }
}

void AutoClicker::setInterval(unsigned long interval) {
  clickInterval = max(10UL, interval);
}

unsigned long AutoClicker::getInterval() const {
  return clickInterval;
}

bool AutoClicker::isEnabled() const {
  return enabled;
}

void AutoClicker::reset() {
  lastClickTime = millis();
  mouseIsPressed = false;
}
