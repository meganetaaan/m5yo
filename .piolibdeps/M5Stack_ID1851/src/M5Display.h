#ifndef _M5DISPLAY_H_
#define _M5DISPLAY_H_

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include "utility/In_eSPI.h"

typedef enum {
  JPEG_DIV_NONE,
  JPEG_DIV_2,
  JPEG_DIV_4,
  JPEG_DIV_8,
  JPEG_DIV_MAX
} jpeg_div_t;

class M5Display : public TFT_eSPI {

 public:
  M5Display();
  void begin();
  void sleep();
  void setBrightness(uint8_t brightness);
  void clearDisplay(uint32_t color=ILI9341_BLACK) { fillScreen(color); }
  void clear(uint32_t color=ILI9341_BLACK) { fillScreen(color); }
  void display() {}

  inline void startWrite() __attribute__((always_inline));
  inline void endWrite() __attribute__((always_inline));
  inline void writePixels(uint16_t *colors, uint32_t len);

  void qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
  void qrcode(const String &string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);

  void drawBmp(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
  void drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

  void drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x = 0,
               uint16_t y = 0, uint16_t maxWidth = 0, uint16_t maxHeight = 0,
               uint16_t offX = 0, uint16_t offY = 0,
               jpeg_div_t scale = JPEG_DIV_NONE);

  void drawJpg(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                   uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                   uint16_t offX = 0, uint16_t offY = 0,
                   jpeg_div_t scale = JPEG_DIV_NONE);

  void drawJpgFile(fs::FS &fs, const char *path, uint16_t x = 0, uint16_t y = 0,
                   uint16_t maxWidth = 0, uint16_t maxHeight = 0,
                   uint16_t offX = 0, uint16_t offY = 0,
                   jpeg_div_t scale = JPEG_DIV_NONE);

 private:
};

#endif
