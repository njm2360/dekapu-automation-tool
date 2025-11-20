#include "RoombaController.h"

constexpr unsigned long TIME_LEFT_ALIGN = 3000;
constexpr unsigned long TIME_SPAWN_ADJUST = 1000;
constexpr unsigned long TIME_BACK_ALIGN = 1500;
constexpr unsigned long TIME_BACK_OTHER = 500;
constexpr unsigned long TIME_FORWARD = 1200;
constexpr unsigned long TIME_RIGHT_SHORT = 160;
constexpr unsigned long TIME_BACK_LONG = 1400;

constexpr unsigned int TASK_GET_CLICK_COUNT = 20;

RoombaController::RoombaController()
  : enabled(false),
    firstSequence(true),
    sequenceTimer(0),
    state(STATE_LEFT_ALIGN),
    scanCount(0),
    clickCount(0),
    mousePressStart(0),
    mouseIsPressed(false) {}

void RoombaController::enable() {
  enabled = true;
  firstSequence = true;
  state = STATE_LEFT_ALIGN;
  scanCount = 0;
  clickCount = 0;
  sequenceTimer = millis();
  Keyboard.press(KEY_LEFT_SHIFT);
}

void RoombaController::disable() {
  enabled = false;
  releaseAllKeys();
}

void RoombaController::update() {
  if (!enabled) return;

  unsigned long now = millis();

  switch (state) {
    // 原点合わせ
    case STATE_LEFT_ALIGN:
      Keyboard.press('A');
      if (now - sequenceTimer >= TIME_LEFT_ALIGN) {
        Keyboard.release('A');
        sequenceTimer = now;
        state = STATE_BACK_ALIGN;
      }
      break;

    case STATE_BACK_ALIGN:
      Keyboard.press('S');
      if (now - sequenceTimer >= (firstSequence ? TIME_BACK_ALIGN : TIME_BACK_OTHER)) {
        Keyboard.release('S');
        sequenceTimer = now;
        scanCount = 0;
        clickCount = 0;
        state = STATE_CLICK;
        firstSequence = false;
      }
      break;

    // タスク回収（クリック）
    case STATE_CLICK:
      if (!mouseIsPressed) {
        Mouse.press(MOUSE_LEFT);
        mouseIsPressed = true;
        mousePressStart = now;
      }
      if (mouseIsPressed && (now - mousePressStart >= 120)) {
        Mouse.release(MOUSE_LEFT);
        mouseIsPressed = false;
        sequenceTimer = now;
        state = STATE_CLICK_WAIT;
      }
      break;

    case STATE_CLICK_WAIT:
      if (now - sequenceTimer >= 120) {
        clickCount++;

        if (clickCount < TASK_GET_CLICK_COUNT) {
          state = STATE_CLICK;
        } else {
          state = STATE_SPAWN_ADJUST;
        }

        sequenceTimer = now;
      }
      break;

    // 部屋の右半分にしか湧かないのでずらす
    case STATE_SPAWN_ADJUST:
      Keyboard.press('D');
      if (now - sequenceTimer >= TIME_SPAWN_ADJUST) {
        Keyboard.release('D');
        sequenceTimer = now;
        state = STATE_FORWARD;
      }
      break;

    case STATE_FORWARD:
      Keyboard.press('W');
      if (now - sequenceTimer >= TIME_FORWARD) {
        Keyboard.release('W');
        sequenceTimer = now;
        state = STATE_RIGHT1;
      }
      break;

    case STATE_RIGHT1:
      Keyboard.press('D');
      if (now - sequenceTimer >= TIME_RIGHT_SHORT) {
        Keyboard.release('D');
        sequenceTimer = now;
        state = STATE_BACK;
      }
      break;

    case STATE_BACK:
      Keyboard.press('S');
      if (now - sequenceTimer >= TIME_BACK_LONG) {
        Keyboard.release('S');
        scanCount++;
        sequenceTimer = now;
        state = (scanCount >= SCAN_COUNT) ? STATE_LEFT_ALIGN : STATE_RIGHT2;
      }
      break;

    case STATE_RIGHT2:
      Keyboard.press('D');
      if (now - sequenceTimer >= TIME_RIGHT_SHORT) {
        Keyboard.release('D');
        sequenceTimer = now;
        state = STATE_FORWARD;
      }
      break;
  }
}

bool RoombaController::isEnabled() const {
  return enabled;
}

void RoombaController::releaseAllKeys() {
  Keyboard.release('A');
  Keyboard.release('S');
  Keyboard.release('W');
  Keyboard.release('D');
  Keyboard.release(KEY_LEFT_SHIFT);
}
