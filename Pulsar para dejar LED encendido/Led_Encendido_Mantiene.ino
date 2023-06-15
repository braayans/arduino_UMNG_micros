bool valor=false;
int estadoAnterior=0;
int salida=0;
void setup() {
  pinMode(50,INPUT);
  pinMode(52,OUTPUT);
  pinMode(53,OUTPUT);

}

void loop() {
  valor=digitalRead(50); //Da como estado HIGH O LOW
  
  if(valor==true && estadoAnterior==false){
    salida=1-salida;
  }

  estadoAnterior=valor;

  if(salida==1){
    digitalWrite(52,HIGH);
    delay(500);
    digitalWrite(52,LOW);
    delay(500);

  }
  else{
    digitalWrite(52,LOW);
  }  
}
