void setup() {
  DDRC=0xFF;

}

void loop() {
  for(int i=0;i<255;i++){
      PORTC=i;
      delayMicroseconds(2);
  }

}
