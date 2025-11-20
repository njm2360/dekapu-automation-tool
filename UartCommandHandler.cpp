#include "UartCommandHandler.h"

UartCommandHandler::UartCommandHandler(HardwareSerial& serial,
                                       AutoClicker& clicker,
                                       AutoInserter& inserter,
                                       RoombaController& roomba,
                                       DisplayUpdateCallback cb)
  : uart(serial), clicker(clicker), inserter(inserter), roomba(roomba),
    updateCb(cb), lastWasCR(false) {}

void UartCommandHandler::update() {
  while (uart.available()) {
    char c = uart.read();

    if (c == '\r' || c == '\n') {
      if (c == '\n' && lastWasCR) {
        lastWasCR = false;
        continue;
      }
      lastWasCR = (c == '\r');

      uart.print("\r\n");
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
      uart.print("> ");
    } else if (c == 0x08 || c == 0x7F) {
      if (inputBuffer.length() > 0) {
        inputBuffer.remove(inputBuffer.length() - 1);
        uart.print("\b \b");
      }
    } else if (isPrintable(c)) {
      inputBuffer += c;
      uart.write(c);
      lastWasCR = false;
    }
  }
}

void UartCommandHandler::processCommand(const String& cmd) {
  if (cmd.startsWith("click interval ")) {
    unsigned long val = cmd.substring(15).toInt();
    clicker.setInterval(val);
  } else if (cmd == "click enable") {
    clicker.enable();
  } else if (cmd == "click disable") {
    clicker.disable();
  } else if (cmd == "insert enable") {
    inserter.enable();
  } else if (cmd == "insert disable") {
    inserter.disable();
  } else if (cmd == "roomba enable") {
    roomba.enable();
  } else if (cmd == "roomba disable") {
    roomba.disable();
  } else if (cmd == "show status") {
    showStatus();
  } else {
    uart.println("Unknown command");
  }

  if (updateCb) updateCb();
}

void UartCommandHandler::showStatus() {
  uart.printf("Click : %s\r\n", clicker.isEnabled() ? "Enabled" : "Disabled");
  uart.printf("  Interval : %lu ms\r\n", clicker.getInterval());
  uart.printf("Insert : %s\r\n", inserter.isEnabled() ? "Enabled" : "Disabled");
  uart.printf("Roomba : %s\r\n", roomba.isEnabled() ? "Enabled" : "Disabled");
}
