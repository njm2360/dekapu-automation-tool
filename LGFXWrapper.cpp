#include "LGFXWrapper.h"

LGFX::LGFX(void) {
  {
    auto cfg = _bus.config();
    cfg.spi_host = 0;
    cfg.spi_mode = 0;
    cfg.freq_write = 80000000;
    cfg.freq_read = 4000000;
    cfg.pin_sclk = TFT_SCLK;
    cfg.pin_mosi = TFT_MOSI;
    cfg.pin_miso = TFT_MISO;
    cfg.pin_dc = TFT_DC;
    _bus.config(cfg);
    _panel.setBus(&_bus);
  }

  {
    auto cfg = _panel.config();
    cfg.pin_cs = -1;
    cfg.pin_rst = -1;
    cfg.pin_busy = -1;
    cfg.panel_width = 240;
    cfg.panel_height = 240;
    cfg.offset_x = 0;
    cfg.offset_y = 0;
    cfg.invert = true;
    _panel.config(cfg);
  }

  setPanel(&_panel);
}
