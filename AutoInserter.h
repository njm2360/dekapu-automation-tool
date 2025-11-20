#pragma once
#include <Arduino.h>
#include <Keyboard.h>

class AutoInserter {
public:
  AutoInserter();
  void enable();
  void disable();
  void toggle();
  void update();
  bool isEnabled() const;

private:
  void reset();

  bool enabled;
  unsigned long keyInterval;
  unsigned long keyPressDuration;
  unsigned long lastKeyTime;
  bool keyIsPressed;
  unsigned long keyPressStartTime;
};
