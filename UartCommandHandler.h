#pragma once
#include <Arduino.h>
#include "AutoClicker.h"
#include "AutoInserter.h"
#include "RoombaController.h"

class UartCommandHandler {
public:
  using DisplayUpdateCallback = void (*)();
  UartCommandHandler(
    HardwareSerial&,
    AutoClicker&,
    AutoInserter&,
    RoombaController&,
    DisplayUpdateCallback cb = nullptr);

  void update();

private:
  void processCommand(const String& cmd);
  void showStatus();

  HardwareSerial& uart;
  AutoClicker& clicker;
  AutoInserter& inserter;
  RoombaController& roomba;
  String inputBuffer;
  bool lastWasCR;
  DisplayUpdateCallback updateCb;
};
