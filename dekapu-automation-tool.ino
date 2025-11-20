#include <Arduino.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce2.h>
#include "LGFXWrapper.h"
#include "AutoInserter.h"
#include "AutoClicker.h"
#include "RoombaController.h"
#include "UartCommandHandler.h"

#define UART_TX_PIN 0
#define UART_RX_PIN 1

LGFX display;
LGFX_Sprite sprite(&display);

AutoInserter inserter;
AutoClicker clicker;
RoombaController roomba;
UartCommandHandler* cmdHandler;

constexpr int buttonA = 3;
constexpr int buttonB = 11;
constexpr int buttonC = 4;
constexpr int buttonD = 12;
constexpr int buttonE = 13;

Bounce debA = Bounce();
Bounce debB = Bounce();
Bounce debC = Bounce();
Bounce debD = Bounce();
Bounce debE = Bounce();

unsigned long lastRepeatA = 0;
unsigned long pressStartA = 0;
bool wasPressedA = false;

unsigned long lastRepeatB = 0;
unsigned long pressStartB = 0;
bool wasPressedB = false;

void updateDisplay() {
  sprite.clear(TFT_BLACK);

  int x = 5;
  int y = 5;
  int w = 230;
  int h = 60;
  int gap = 10;

  // Auto Click
  sprite.drawRect(x, y, w, h, TFT_WHITE);
  sprite.setTextColor(TFT_WHITE);
  sprite.setCursor(x + 8, y + 5);
  sprite.printf("Click : %lums", clicker.getInterval());

  if (clicker.isEnabled()) {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_GREEN);
    sprite.setTextColor(TFT_BLACK);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Enable");
  } else {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_RED);
    sprite.setTextColor(TFT_WHITE);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Disable");
  }
  sprite.setTextSize(2);
  y += h + gap;

  // Auto Insert
  sprite.drawRect(x, y, w, h, TFT_WHITE);
  sprite.setTextColor(TFT_WHITE);
  sprite.setCursor(x + 8, y + 5);
  sprite.print("Medal Insert");

  if (inserter.isEnabled()) {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_GREEN);
    sprite.setTextColor(TFT_BLACK);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Enable");
  } else {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_RED);
    sprite.setTextColor(TFT_WHITE);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Disable");
  }
  sprite.setTextSize(2);
  y += h + gap;

  // Roomba
  sprite.drawRect(x, y, w, h, TFT_WHITE);
  sprite.setTextColor(TFT_WHITE);
  sprite.setCursor(x + 8, y + 5);
  sprite.print("Sherbi Roomba");

  if (roomba.isEnabled()) {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_GREEN);
    sprite.setTextColor(TFT_BLACK);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Enable");
  } else {
    sprite.fillRect(x + 8, y + 25, w - 16, 25, TFT_RED);
    sprite.setTextColor(TFT_WHITE);
    sprite.setTextSize(3);
    sprite.setCursor(x + 50, y + 27);
    sprite.print("Disable");
  }

  sprite.setTextSize(2);
  sprite.pushSprite(0, 0);
}

void setup() {
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_SCLK, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  digitalWrite(TFT_SCLK, HIGH);
  digitalWrite(TFT_RST, HIGH);

  display.init();
  sprite.setColorDepth(8);
  sprite.createSprite(240, 240);
  sprite.setTextSize(2);
  sprite.setTextColor(TFT_WHITE);
  updateDisplay();

  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);

  debA.attach(buttonA);
  debA.interval(10);
  debB.attach(buttonB);
  debB.interval(10);
  debC.attach(buttonC);
  debC.interval(50);
  debD.attach(buttonD);
  debD.interval(50);
  debE.attach(buttonE);
  debE.interval(50);

  Serial1.setTX(UART_TX_PIN);
  Serial1.setRX(UART_RX_PIN);
  Serial1.begin(115200);
  
  Keyboard.begin();
  Mouse.begin();

  cmdHandler = new UartCommandHandler(Serial1, clicker, inserter, roomba, updateDisplay);
}

void loop() {
  unsigned long now = millis();

  // ボタンA: 間隔短く
  debA.update();
  if (debA.read() == LOW) {
    if (!wasPressedA) {
      pressStartA = now;
      wasPressedA = true;
    }
    unsigned long held = now - pressStartA;
    if (held < 1000) {
      if (debA.fell()) {
        clicker.setInterval(clicker.getInterval() - 10);
        updateDisplay();
      }
    } else {
      unsigned long interval = 200UL;
      if (held > 1000) {
        unsigned long delta = (held - 1000) / 10;
        interval = (delta >= interval - 10) ? 10 : interval - delta;
      }
      unsigned long delta = (held - 1000) / 1000;
      unsigned long step = 10UL << delta;

      if (now - lastRepeatA >= interval) {
        clicker.setInterval(clicker.getInterval() - step);
        updateDisplay();
        lastRepeatA = now;
      }
    }
  } else {
    wasPressedA = false;
  }

  // ボタンB: 間隔長く
  debB.update();
  if (debB.read() == LOW) {
    if (!wasPressedB) {
      pressStartB = now;
      wasPressedB = true;
    }
    unsigned long held = now - pressStartB;
    if (held < 1000) {
      if (debB.fell()) {
        clicker.setInterval(clicker.getInterval() + 10);
        updateDisplay();
      }
    } else {
      unsigned long interval = 200UL;
      if (held > 1000) {
        unsigned long delta = (held - 1000) / 10;
        interval = (delta >= interval - 10) ? 10 : interval - delta;
      }
      unsigned long delta = (held - 1000) / 1000;
      unsigned long step = 10UL << delta;

      if (now - lastRepeatB >= interval) {
        clicker.setInterval(clicker.getInterval() + step);
        updateDisplay();
        lastRepeatB = now;
      }
    }
  } else {
    wasPressedB = false;
  }

  // C: 自動クリックトグル
  debC.update();
  if (debC.fell()) {
    clicker.toggle();
    updateDisplay();
  }

  // D: 自動投入トグル
  debD.update();
  if (debD.fell()) {
    inserter.toggle();
    updateDisplay();
  }

  // E: ルンバ機能トグル
  debE.update();
  if (debE.fell()) {
    if (roomba.isEnabled()) roomba.disable();
    else roomba.enable();
    updateDisplay();
  }

  inserter.update();
  clicker.update();
  roomba.update();

  cmdHandler->update();
}
