#pragma once
#include <LovyanGFX.hpp>

#define TFT_SCLK 18
#define TFT_MOSI 19
#define TFT_MISO -1
#define TFT_DC 17
#define TFT_RST 20

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX(void);
};
