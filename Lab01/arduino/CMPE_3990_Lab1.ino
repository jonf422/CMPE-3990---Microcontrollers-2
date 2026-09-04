#define POT_PIN A2
#define DATA_PIN 2
#define CLOCK_PIN 3
#define LATCH_PIN 0
#define OE_PIN 1

void setup() {
  // put your setup code here, to run once:
  pinMode(POT_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t pot_in = analogRead(POT_PIN);
  uint8_t level = (8*pot_in)/1023;
  uint8_t out = (uint8_t)((1u << level) - 1);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, out);
  digitalWrite(LATCH_PIN, HIGH);

  delay(20);
}
