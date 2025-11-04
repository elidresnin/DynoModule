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

bool first_start;


// Declare current value global variables
bool enable = false;
bool disable = true;
int torque_value = 0;
bool reverse = false;

// 
void setup(void) {

  pinMode(FORREV_PIN, INPUT_PULLUP);
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  pinMode(ENGINESTART_PIN, INPUT_PULLUP);
  pinMode(ONOFF_PIN, INPUT_PULLUP);

  u8g2.begin();
  first_start = true;

  
}



void loop(void) {
  // Clears screen and set font
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  // Show first start graphic for 1 second
  if(first_start == true) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 50);
    u8g2.print("TREV DYNO MODULE");
    u8g2.sendBuffer();
    delay(1000);
    first_start = false;
  }

  // Set global variables
  reverse = digitalRead(FORREV_PIN);
  
  enable = !digitalRead(ENABLE_PIN);
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  torque_value = map(potentiometerValue, 0, 1023, 0, 220);


  if (digitalRead(ESTOP_PIN) == HIGH) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 50);
    u8g2.print("ESTOP!");

  } else {

    u8g2.setCursor(0, 10);  // Set cursor position (x, y)
    u8g2.print("Torque Value: ");  // Print a string label
    u8g2.print(torque_value);  // write something to the internal memory
    // Serial.print("Potentiometer Value: ");
    // Serial.println(potentiometerValue);


    

    // Reverse
    u8g2.setCursor(0, 20);
    u8g2.print(reverse ? "Direction: Reverse" : "Direction: Forward");

    // enable
    u8g2.setCursor(0, 30);
    u8g2.print(enable ? "Enabled" : "Disabled");
    



    // if (enable) {
    //   u8g2.setCursor(0, 30);
    //   u8g2.print("Enabled");
      

    // } else {
    //   u8g2.setCursor(0, 30);
    //   u8g2.print("Disabled");
      
    // }



    if (digitalRead(ONOFF_PIN) == HIGH) {

      u8g2.setCursor(0, 50);

      u8g2.print("Power OFF");

    } else {

      u8g2.setCursor(0, 50);

      u8g2.print("Power ON");
    }
  }

  u8g2.sendBuffer();  // transfer internal memory to the display
  first_start = false;
  delay(50);
  
}
