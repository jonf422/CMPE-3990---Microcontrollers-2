#include <Adafruit_DotStar.h>

#define DAC_PIN A0
#define TRIG_PIN 4

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

const int8_t dac_res = 10;
const int16_t dac_max = (1u << dac_res) - 1;

const int N = 512;

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
  for (int i = 0; i < N; i++) {
    float phase = (2.0f * PI * i) / N;
    uint16_t val = (uint16_t)((sinf(phase) * 0.5f + 0.5f) * dac_max);
    analogWrite(DAC_PIN, val);
  }
}

//freq = 14.608 Hz