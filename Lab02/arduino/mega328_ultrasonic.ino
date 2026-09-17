#include "Arduino.h"
#include <U8g2lib.h>
#include <DHT11.h>

#define DHT_PIN 2
#define TEMP_BUTTON_PIN 11
#define TRIG_PIN 3
#define ECHO_PIN 4

DHT11 dht11(DHT_PIN);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//U8G2 display vars
bool use_fahrenheit = false;
bool dirty_display = true;

//button vars
const unsigned long debounce_ms = 40;
bool temp_button_state = true;   //idle = HIGH with INPUT_PULLUP
bool temp_last_button = true;
unsigned long temp_last_press_ms = 0;

//ultrasonic / motion vars
unsigned long last_ping = 0;
const long motion_threshold_cm = 10;       //change needed to count as motion
long last_dist = -1;


unsigned long last_read = 0;
const unsigned long read_int = 1000;
int last_temp_c = 0;
bool last_valid = false;

void checkMotion(){
  unsigned long now = millis();
  if (now - last_ping < 100) return;
  last_ping = now;

  digitalWrite(TRIG_PIN, HIGH); //pulse trig pin to start reading
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long dur = pulseIn(ECHO_PIN, HIGH, 25000UL);
  long curr_dist = (long)(dur / 58);
  if (dur == 0) return;

  if (last_dist >= 0) {
    long diff = curr_dist - last_dist;
    if (diff < 0) diff = -diff;
    if (diff >= motion_threshold_cm) {
      dirty_display = true;
    }
  }

  last_dist = curr_dist;
}

void handleTempButton(){

  //read TEMP_BUTTON_PIN
  bool read = digitalRead(TEMP_BUTTON_PIN);

  //check if button state change
  if (read != temp_last_button){
    temp_last_press_ms = millis();
  }

  //check debounce time buffer
  if ((millis() - temp_last_press_ms) > debounce_ms) {
    if (read != temp_button_state){
      temp_button_state = read;
      if (temp_button_state == LOW){   //pressed
        use_fahrenheit = !use_fahrenheit;
        dirty_display = true;
      }
    }
  }
  temp_last_button = read;
}

void readSensor(){
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

void updateDisplay(){
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);

    u8g2.setCursor(10, 28);
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

    u8g2.setCursor(10, 52);
    if (last_dist >= 0) {
      u8g2.print("Dist: ");
      u8g2.print(last_dist);
      u8g2.print(" cm");
    } else {
      u8g2.print("Dist: --");
    }
  } while (u8g2.nextPage());
  dirty_display = false;
}

void setup(){
  u8g2.begin();
  pinMode(TEMP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop(){
  handleTempButton();
  checkMotion();
  readSensor();
  if (dirty_display) updateDisplay();
}