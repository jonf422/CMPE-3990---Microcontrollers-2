#include "Arduino.h"
#include <U8g2lib.h>
#include <DHT11.h>

#define DHT_PIN 2
#define BUTTON_PIN 11

DHT11 dht11(DHT_PIN);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//U8G2 display vars
bool use_fahrenheit = false;
bool dirty_display = true;

//button vars
bool button_state = true;   //idle = HIGH with INPUT_PULLUP
bool last_button = true;
unsigned long last_press_ms = 0;
const unsigned long debounce_ms = 40;

//DHT11 temp sensor vars
unsigned long last_read = 0;
const unsigned long read_int = 1000;

int last_temp_c = 0;
bool last_valid = false;

void handleButton() {
 
  //read BUTTON_PIN
  bool read = digitalRead(BUTTON_PIN);

  //check if button state change
  if (read != last_button) {
    last_press_ms = millis();
  }

  //check debounce time buffer
  if ((millis() - last_press_ms) > debounce_ms) {
    if (read != button_state) {
      button_state = read;
      if (button_state == LOW) {   //pressed
        use_fahrenheit = !use_fahrenheit;
        dirty_display = true;
      }
    }
  }

  last_button = read;
}

void readSensor() {
  unsigned long now = millis();
  if (now - last_read < read_int) return;
  last_read = now;

  int temperature = dht11.readTemperature();
  bool valid = (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT);

  if (valid && last_temp_c != temperature) {
    dirty_display = true;
  }
  if (valid != last_valid) {
    dirty_display = true;
  }

  last_valid = valid;
  if (valid) last_temp_c = temperature;
}

void updateDisplay() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setCursor(10, 50);
    if (last_valid) {
      if (use_fahrenheit) {
        int f = (9 * last_temp_c) / 5 + 32;
        u8g2.print("Temp: ");
        u8g2.print(f);
        u8g2.print(" F");
      } else {
        u8g2.print("Temp: ");
        u8g2.print(last_temp_c);
        u8g2.print(" C");
      }
    } else {
      u8g2.print("Sensor error");
    }
  } while (u8g2.nextPage());
  dirty_display = false;
}

void setup() {
  u8g2.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  handleButton();
  readSensor();
  if (dirty_display) updateDisplay();
}