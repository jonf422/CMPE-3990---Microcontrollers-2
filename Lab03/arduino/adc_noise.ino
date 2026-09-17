#include <Adafruit_DotStar.h>

Adafruit_DotStar strip(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(80);
  strip.setPixelColor(0,0,0,0);
  strip.show();

  Serial.begin(115200);
  while(!Serial){;}
  analogReadResolution(12);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t time = micros();
  uint16_t vals[2000];
  uint16_t time_vals[2000];
  for(int i=0; i<2000; i++){
    time_vals[i] = micros()-time;
    vals[i] = analogRead(2);
  }

  for(int i=0; i<2000; i++){
    Serial.printf("%d,%d\n", time_vals[i], vals[i]);
  }
  Serial.print("OK");

  delay(50000);

}
