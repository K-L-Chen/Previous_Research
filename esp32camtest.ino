#define ONBOARD_LED  2

void setup() {
  pinMode(ONBOARD_LED,OUTPUT);
}

void loop() {
  delay(5000);
  digitalWrite(ONBOARD_LED,HIGH);
  delay(5000);
  digitalWrite(ONBOARD_LED,LOW);
}