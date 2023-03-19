int valorDigital=204;
void setup() {
  DDRC = 0xFF;
  Serial.begin(9600);
}

void loop() {
  PORTC=valorDigital;
  Serial.println(PORTC);
  delay(1000);
  
}
