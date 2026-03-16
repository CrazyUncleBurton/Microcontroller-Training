/*

  M5Stack Tab5 Hardware ID

  by Bryan A. "CrazyUncleBurton" Thompson

  Last Updated 2/19/2026

*/

#include <M5Unified.h>
#if __has_include(<lgfx/v1/platforms/esp32p4/Panel_DSI.hpp>)
#include <lgfx/v1/platforms/esp32p4/Panel_DSI.hpp>
#endif

M5GFX& display = M5.Display;


// Variables
int lineHeight = display.fontHeight() + 14;;
int x_margin = 25; 
int y_header = 25;
int x = x_margin;
int y = y_header; 
// const int i2cSdaPin = 53;
// const int i2cSclPin = 54;
const int textSize = 2;


const char* detectLcdController(m5::board_t board, lgfx::Panel_Device* panel)
{
  if (!panel)
  {
    return "Unknown";
  }

#if __has_include(<lgfx/v1/platforms/esp32p4/Panel_DSI.hpp>)
  if (board == m5::board_t::board_M5Tab5)
  {
    auto bus = panel->getBus();
    if (!bus || bus->busType() != lgfx::bus_type_t::bus_dsi)
    {
      return "Unknown (No DSI Panel)";
    }

    auto dsi_panel = static_cast<lgfx::Panel_DSI*>(panel);
    auto detail = dsi_panel->config_detail();
    if (detail.vsync_front_porch >= 200)
    {
      return "ST7123";
    }
    if (detail.vsync_front_porch <= 40)
    {
      return "ILI9881C";
    }

    return "Unknown (Tab5 DSI)";
  }
#endif

  return "Unknown";
}


const char* boardToString(m5::board_t board)
{
  switch (board)
  {
    case m5::board_t::board_M5Tab5: return "M5Stack Tab5";
    case m5::board_t::board_M5Stack: return "M5Stack";
    case m5::board_t::board_M5StackCore2: return "M5Stack Core2";
    case m5::board_t::board_M5StackCoreS3: return "M5Stack CoreS3";
    case m5::board_t::board_M5StackCoreS3SE: return "M5Stack CoreS3SE";
    default: return "Unknown";
  }
}


void scanBus(m5::I2C_Class& bus, const char* busName)
{
  display.setCursor(x,y);
  display.printf("Scanning %s: ", busName);
  int found = 0;
   for (uint8_t addr = 0x08; addr <= 0x77; addr++)
  {
    if (bus.scanID(addr))
    {
      display.printf("0x%02X ", addr);
      found++;
    }
  }
  if (found == 0)
  {
    display.printf("None Found.");
  }
  y += lineHeight*2;
}


void setup()
{
  // Start M5 Libraries which support the Tab5 (includes serial)
  auto cfg = M5.config();
  M5.begin(cfg);

  // Start I2C library
  // Wire1.begin(i2cSdaPin, i2cSclPin);
  // Make sure both buses are up (harmless if already initialized)
  M5.In_I2C.begin();
  M5.Ex_I2C.begin();

  // Configure the LCD properties
  display.setRotation(1);
  display.setBrightness(255);

  // Configure direct LCD text rendering
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setFont(&fonts::efontJA_24_b);
  display.setTextSize(textSize);
  display.setTextWrap(false, false);
  lineHeight = display.fontHeight() + 14;

  // Draw the Info Screen
  auto panel = M5.Display.getPanel();
  auto board = M5.getBoard();
  auto controller = detectLcdController(board, panel);
  display.fillScreen(TFT_BLACK);
  display.setTextSize(textSize);
  display.setCursor(x, y);
  display.printf("Development Board: %s", boardToString(board));
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("LCD Controller: %s", controller);
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("LCD Resolution: %dx%d pixels", M5.Display.width(), M5.Display.height());
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("LCD Max Touch Points: %d", m5::Touch_Class::TOUCH_MAX_POINTS);
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("MCU: %s Rev%d @ %dMHz", ESP.getChipModel(), ESP.getChipRevision(), ESP.getCpuFreqMHz());
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("Flash: %u MB", ESP.getFlashChipSize()/(1024*1024));
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("PSRAM: %u MB", ESP.getPsramSize()/(1024*1024));
  y += lineHeight;
  display.setCursor(x, y);
  display.printf("Battery: %.2fV", M5.Power.getBatteryVoltage());
  display.setTextWrap(true);
  scanBus(M5.In_I2C, "Internal I2C Bus");
  scanBus(M5.Ex_I2C, "External I2C Bus");

}

void loop()
{


}
