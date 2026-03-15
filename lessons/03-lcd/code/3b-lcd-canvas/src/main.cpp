/*

  M5Stack Tab5 LCD + Canvas Tutorial

  by Bryan A. "CrazyUncleBurton" Thompson

  Last Updated 3/14/2026

*/

// Include the M5Unified library and the sensor libraries in our project. These 
// libraries contain the code we need to interact with the hardware of the 
// M5Stack Tab5 and the sensors we are using.
#include <M5Unified.h>
#include "M5UnitENV.h"

// Give these objects names so we can refer to them in the code.
M5GFX& display = M5.Display;
M5Canvas canvas(&display);
SHT3X sht3x;
QMP6988 qmp;

// Global Variables
const int i2cSdaPin = 53;
const int i2cSclPin = 54;
const int textSize = 4;
float shtTempF = 0.0f;
float shtHumidity = 0.0f;
float qmpTempF = 0.0f;
float qmpPressureInHg = 0.0f;
float qmpAltitudeFt = 0.0f;
bool shtPresent = false;
bool qmpPresent = false;
int width = display.width();
int height = display.height();
const char degreeSymbol[] = { static_cast<char>(0xC2), static_cast<char>(0xB0), '\0' };

// The info will be drawn on the canvas and then pushed to the display. This 
// allows us to update the entire screen at once, which can help reduce 
// flickering and improve performance.
void drawDashboard()
{
  // Clear the canvas and set up text properties
  canvas.fillScreen(TFT_BLACK);
  canvas.setTextSize(textSize);
  canvas.setTextColor(TFT_WHITE, TFT_BLACK);

  // Lets make it easy to position the text by using a line height variable and 
  // incrementing the y position for each line of text.
  int lineHeight = canvas.fontHeight() + 14;
  int x = 175;
  int y = 25;
  canvas.setCursor(x, y);

  // Draw the title of our dashboard at the top of the screen.
  canvas.printf("My Weather Station");

  // Jump to the next line for the sensor data. 
  y += lineHeight;
  
  // Check if the sensors are present before trying to display their data. 
  if ((shtPresent)&&(qmpPresent)) {
    
    // Change the color of the text to green for the sensor data
    canvas.setTextColor(TFT_GREEN, TFT_BLACK);
    canvas.setCursor(x, y);
    
    // Display the data for each sensor. We use printf formatting to 
    //format the data with units and a consistent number of decimal places.
    canvas.printf("  Temp 1: %.2f%sF", shtTempF, degreeSymbol);
    y += lineHeight;
    canvas.setCursor(x, y);
    canvas.printf("  Temp 2: %.2f%sF", qmpTempF, degreeSymbol);
    y += lineHeight;
    canvas.setCursor(x, y);
    canvas.printf("Humidity: %.2f%% RH", shtHumidity);
    y += lineHeight;
    canvas.setCursor(x, y);
    canvas.printf("Pressure: %.2f inHg", qmpPressureInHg);
    y += lineHeight;
    canvas.setCursor(x, y);
    canvas.printf("Altitude: %.2f ft", qmpAltitudeFt);

  } else { // Display an error message if one or both of the sensors are not present.
    canvas.setCursor(x, y);
    canvas.setTextColor(TFT_RED, TFT_BLACK);
    canvas.printf("ENV III not found");
  }

  // Push the canvas to the display. This will update the entire screen with the new 
  // information we have drawn on the canvas.
  canvas.pushSprite(0, 0);
}

void setup()
{
  // Initialize the M5Stack Tab5. The configuration options are set in the 
  //M5.begin() function, but you can also set them in the M5.config struct 
  //if you want to change the default settings.
  auto cfg = M5.config();
  M5.begin(cfg);

  // Initialize the I2C bus. The sensors we are using communicate with the 
  //M5Stack Tab5 over I2C, so we need to initialize the I2C bus before we 
  //can communicate with the sensors.
  Wire1.begin(i2cSdaPin, i2cSclPin);

  // Set up the display. We will use landscape mode for this dashboard.
  display.setRotation(1); // set landscape mode, USB Port to the right

  // Set up the canvas. We will use the canvas to draw the dashboard 
  //and then push it to the display. This allows us to update the entire 
  //screen at once, which can help reduce flickering and improve performance.
  canvas.setColorDepth(16);
  canvas.createSprite(display.width(), display.height());
  canvas.setTextColor(TFT_WHITE, TFT_BLACK);
  canvas.setFont(&fonts::efontJA_24_b); // This is a Unicode font that includes our degrees symbol.  
  canvas.setTextSize(textSize);
  canvas.setTextWrap(false, false);

  // Draw the initial screen. This will show a title and a message 
  //indicating that the program is starting. We will then update this 
  //screen with the sensor data in the loop() function.
  canvas.fillScreen(TFT_BLACK);
  canvas.drawCenterString("M5Stack Tab5", display.width() / 2, display.height() / 2 - 150);
  canvas.drawCenterString("Starting...", display.width() / 2 + 20, display.height() / 2 + 50);
  canvas.pushSprite(0, 0);
  delay(1000);

  // Initialize the sensors. The begin() function will return true if the 
  //sensor is present and initialized successfully, and false if the sensor 
  //is not present or there was an error initializing it. We will store 
  //this information in the shtPresent and qmpPresent variables so we can 
  //check it later when we try to read data from the sensors.
  qmpPresent = qmp.begin(&Wire1, QMP6988_SLAVE_ADDRESS_L, i2cSdaPin, i2cSclPin, 100000U);
  shtPresent = sht3x.begin(&Wire1, SHT3X_I2C_ADDR, i2cSdaPin, i2cSclPin, 100000U);

  // Draw the initial dashboard. This will show the title and either 
  // the sensor data or an error message if the sensors are not present. 
  // We will then update this dashboard with the latest sensor data in the 
  //loop() function.
  drawDashboard();

}

void loop()
{
  // We will update the sensor data and redraw the dashboard every 
  //second. To do this, we will use the millis() function to check 
  // how much time has passed since the last update, and only update 
  // the dashboard if at least 1000 milliseconds (1 second) has 
  // passed. This will help reduce flickering and improve performance 
  // by not updating the display more often than necessary.
  static uint32_t lastFrameMs = 0;
  const uint32_t frameIntervalMs = 1000;
  if (millis() - lastFrameMs < frameIntervalMs) {
    return;
  }
  lastFrameMs = millis();

  // Update the sensor data. We will check if each sensor is present before 
  // trying to read data from it, and only update the variables if the sensor 
  // is present and the update() function returns true (indicating that new data 
  // was read successfully). This will help prevent errors if one or both of 
  // the sensors are not present or there is an issue communicating with them.
  if (shtPresent && sht3x.update()) {
    shtTempF = (sht3x.cTemp * 1.8f) + 32.0f;
    shtHumidity = sht3x.humidity;
  }
  if (qmpPresent && qmp.update()) {
    qmpTempF = (qmp.cTemp * 1.8f) + 32.0f;
    qmpPressureInHg = qmp.pressure * 0.0002953f;
    qmpAltitudeFt = qmp.altitude * 3.28084f;
  }

  // Draw the dashboard with the updated sensor data. This will update the 
  // entire screen with the new information we have drawn on the canvas.
  drawDashboard();
}
