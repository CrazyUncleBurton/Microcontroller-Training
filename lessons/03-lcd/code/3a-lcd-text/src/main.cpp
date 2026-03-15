/*

  M5Stack Tab5 LCD Text Tutorial

  by Bryan A. "CrazyUncleBurton" Thompson

  Last Updated 3/14/2026

*/

/* 
   Include the M5Unified library and the sensor libraries in our project. 
   These libraries contain the code we need to interact with the hardware 
   of the M5Stack Tab5 and the sensors we are using.  
*/
#include <M5Unified.h>
#include "M5UnitENV.h"

// Give each of the sensor objects a name so we can refer to them in the code.
SHT3X sht3x;
QMP6988 qmp;

// Global Variables
const int i2cSdaPin = 53;
const int i2cSclPin = 54;
float shtTempF = 0.0f;
float shtHumidity = 0.0f;
float qmpTempF = 0.0f;
float qmpPressureInHg = 0.0f;
float qmpAltitudeFt = 0.0f;
bool shtPresent = false;
bool qmpPresent = false;
int width;
int height;

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  // Initialize I2C and sensors
  Wire1.begin(i2cSdaPin, i2cSclPin);

  //display properties
  M5.Display.setRotation(0); // set portrait mode, USB Port at the bottom
  //display.setRotation(1); // set landscape mode, USB Port to the right
  //display.setRotation(2); // set portrait mode, USB Port at the top
  //display.setRotation(3); // set landscape mode, USB Port to the left
  M5.Display.setTextSize(4);  // set text size
  M5.Display.setCursor(20, 20); // set cursor position for the first line of text
  width = M5.Display.width();
  height = M5.Display.height();
  M5.Display.clear(TFT_BLACK);  // Clear the display to start with a blank screen

  // Check for sensors
  qmpPresent = qmp.begin(&Wire1, QMP6988_SLAVE_ADDRESS_L, i2cSdaPin, i2cSclPin, 100000U);
  shtPresent = sht3x.begin(&Wire1, SHT3X_I2C_ADDR, i2cSdaPin, i2cSclPin, 100000U);
  while (!qmpPresent || !shtPresent) {
    M5.Display.setCursor(20, 100);      
    M5.Display.print("Sensors Missing");  // display Hello World! and one line is displayed on the screen
    delay(1000);
    M5.Display.clear(TFT_BLACK); 
  } 
  M5.Display.setCursor(20, 100);      
  M5.Display.drawString("Sensors Found - Continuing!", 20, 20); // Another way to display text
  delay(1000);
  M5.Display.clear(TFT_BLACK); 
                        
}

void loop()
{

  // Update Sensor Readings
  sht3x.update();
  shtTempF = (sht3x.cTemp * 1.8f) + 32.0f;
  shtHumidity = sht3x.humidity;
  qmp.update();
  qmpTempF = (qmp.cTemp * 1.8f) + 32.0f;
  qmpPressureInHg = qmp.pressure * 0.0002953f;
  qmpAltitudeFt = qmp.altitude * 3.28084f;
  
  // Display Header
  M5.Display.setTextColor(TFT_WHITE); // change text color to white
  M5.Display.drawCentreString("My Weather Station", M5.Display.width() / 2, 40);   // A second way of displaying text
  
  // Display Data
  M5.Display.setTextColor(TFT_GREEN); // change text color to green
  M5.Display.setCursor(width/2-225, 150);
  M5.Display.printf("  Temp 1: %.2fF", shtTempF); // A third way of displaying text, using printf formatting. 
  M5.Display.setCursor(width/2-225, 200);
  M5.Display.printf("  Temp 2: %.2fF", qmpTempF);
  M5.Display.setCursor(width/2-225, 250);
  M5.Display.printf("Humidity: %.2f%% RH", shtHumidity);
  M5.Display.setCursor(width/2-225, 300);
  M5.Display.printf("Pressure: %.2f inHg", qmpPressureInHg);
  M5.Display.setCursor(width/2-225, 350);
  M5.Display.printf("Altitude: %.2f ft", qmpAltitudeFt);
  
  // Wait for the user to read it
  delay(3000);

  // Clear the display for the next update
  M5.Display.clear(TFT_BLACK);

}
