#pragma once
#include <Arduino.h>
#include <Mouse.h>
#include <Keyboard.h>

enum RoombaState {
  STATE_LEFT_ALIGN = 0,
  STATE_BACK_ALIGN,
  STATE_CLICK,
  STATE_CLICK_WAIT,
  STATE_SPAWN_ADJUST,
  STATE_FORWARD,
  STATE_RIGHT1,
  STATE_BACK,
  STATE_RIGHT2
};

class RoombaController {
public:
  RoombaController();
  void enable();
  void disable();
  void update();
  bool isEnabled() const;

private:
  void releaseAllKeys();

  static const int SCAN_COUNT = 5;

  bool enabled;
  bool firstSequence;
  unsigned long sequenceTimer;
  RoombaState state;
  int scanCount;

  int clickCount;
  unsigned long mousePressStart;
  bool mouseIsPressed;
};
