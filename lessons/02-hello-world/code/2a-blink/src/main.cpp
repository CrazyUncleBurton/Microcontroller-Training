/*
  Lesson 2A - Hello World
  by Bryan A. "CrazyUncleBurton" Thompson
  Last Updated 3/9/2026
*/

// Includes
#include <Arduino.h> // Add the Arduino library so we can use the Arduino functions like pinMode() and digitalWrite()

// Variables
int greenLEDPin = 53; // Our green LED is connected to pin 53 on the M5Stack
int redLEDPin = 54; // Our red LED is connected to pin 54 on the M5Stack

// The stuff in setup() runs one time when the M5Stack is powered on or reset
void setup()  
{
  pinMode(greenLEDPin, OUTPUT); // set the pin mapped to the greenLEDPin to be an output pin
  pinMode(redLEDPin, OUTPUT); // set the pin mapped to the redLEDPin to be an output pin
}

// The stuff in loop()runs over and over again until the M5Stack is powered off or reset
void loop() 
{
  digitalWrite(greenLEDPin, HIGH); // Turn the green LED on
  digitalWrite(redLEDPin, LOW); // Turn the red LED off
  delay(500); // Wait for half a second
  digitalWrite(greenLEDPin, LOW); // Turn the green LED off
  digitalWrite(redLEDPin, HIGH); // Turn the red LED on
  delay(500); // Wait for half a second
}
