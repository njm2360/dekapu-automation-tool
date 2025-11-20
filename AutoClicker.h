#pragma once
#include <Arduino.h>
#include <Mouse.h>

class AutoClicker {
public:
  AutoClicker();
  void enable();
  void disable();
  void toggle();
  void update();
  void setInterval(unsigned long interval);
  unsigned long getInterval() const;
  bool isEnabled() const;

private:
  void reset();

  bool enabled;
  unsigned long clickInterval;
  const unsigned long mousePressDuration;
  unsigned long lastClickTime;
  bool mouseIsPressed;
  unsigned long mousePressStartTime;
};
