int leer;
void setup() {
  DDRB=0XFF;
  DDRL = 0X00; //Pulsadores puerto L entrada
}

void loop() {
 
  leer=PINL & 0X10;

  if (leer==16){
  digitalWrite(51,HIGH);
  }
  else
  digitalWrite(51,LOW);
}
