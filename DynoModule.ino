/* 
 DynoModule.ino 
*/

// Import Librarys
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// U8G2 Display Constructor
U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/17, /* dc=*/16, /* reset=*/15);

// Define Physical Pins
#define ESTOP_PIN 2
#define FORREV_PIN 3
#define ENABLE_PIN 4
#define ENGINESTART_PIN 5
#define ONOFF_PIN 6
const int POTENTIOMETER_PIN = A0;

// Declare State Variables
bool first_start;

// Declare current value global variables
bool enable = false;
bool send_can = false;
bool disable = true;
int potentiometerValue = 0;
int torque_value = 0;
bool reverse = false;


void setup(void) {
  // Set pin modes (PULLUP = internal resistor)
  pinMode(FORREV_PIN, INPUT_PULLUP);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  pinMode(ENGINESTART_PIN, INPUT_PULLUP);
  pinMode(ONOFF_PIN, INPUT_PULLUP);
  pinMode(ENGINESTART_PIN, INPUT_PULLUP);

  // Initalize Screen
  u8g2.begin();
  first_start = true;
}



void loop(void) {
  // Clears screen and set font
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  // Set global variables
  reverse = digitalRead(FORREV_PIN);
  enable = !digitalRead(ENABLE_PIN);
  potentiometerValue = analogRead(POTENTIOMETER_PIN);
  torque_value = map(potentiometerValue, 0, 1023, 0, 220);

  // Show first start graphic for 1 second
  if(first_start) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 50);
    u8g2.print("TREV DYNO MODULE");
    u8g2.sendBuffer();
    delay(1000);
    first_start = false;
  }

  // ESTOP blocks the rest of the code from running
  // TODO: Send CAN Message to disable
  if (digitalRead(ESTOP_PIN) == HIGH) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 50);
    u8g2.print("ESTOP!");

  } else {
    // Torque
    u8g2.setCursor(0, 10);  // Set cursor position (x, y)
    u8g2.print("Torque Value: ");  // Print a string label
    u8g2.print(torque_value);  // write something to the internal memory

    // Reverse
    u8g2.setCursor(0, 20);
    u8g2.print(reverse ? "Direction: Reverse" : "Direction: Forward");

    // Enable
    u8g2.setCursor(0, 30);
    u8g2.print(enable ? "Enabled" : "Disabled");

    if (digitalRead(ENGINESTART_PIN) == HIGH) {
      
    }
    
  }

  u8g2.sendBuffer();  // transfer internal memory to the display
  first_start = false;
  delay(50);
  
}