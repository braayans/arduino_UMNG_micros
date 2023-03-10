void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(11,OUTPUT);

}

void loop() {
  for(int i=0;i<10;i++){

    if(i%2==0){
      digitalWrite(LED_BUILTIN,HIGH);
      digitalWrite(11,HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(11,LOW);
      delay(500);
    }

    if(i%2!=0){
      digitalWrite(LED_BUILTIN,HIGH);
      digitalWrite(11,HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(11,LOW);
      delay(50);
    }

  }

}
