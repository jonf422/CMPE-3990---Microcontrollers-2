#include <Adafruit_DotStar.h>

#define DAC_PIN A0
#define TRIG_PIN 4

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

const int8_t dac_res = 10;
const int16_t dac_max = (1u << dac_res) - 1;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(80);
  strip.setPixelColor(0,0,0,0);
  strip.show();

  analogWriteResolution(dac_res);
  pinMode(TRIG_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG_PIN, HIGH);
  digitalWrite(TRIG_PIN, LOW);
  for (int i=0; i<dac_max; i++){
    analogWrite(DAC_PIN, i);
    delayMicroseconds(5);
  }

}

// Vertical step: (from oscilliscope) 12.95mV - 9.25mV = 3.7mV
// Frequency: (from oscilliscope) 134.06Hz