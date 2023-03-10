/*                                              REALIZADO POR: BRAYAN ROMERO      

Codigo sencillo de manejo de dos LEDS. Primer pulsacion activa el led pin52 parpadeando. Otra pulsacion 
activa el led pin 53 parpadeando otra pulsacion vuelve a prender el led pin52 y asi sucesivamente.
Para apagar se mantiene presionado el pulsador

*/

const int apagar=7; //Numero de ciclos de espera para que se apague
int espera=0;

bool primerPulso=false;
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
  
  if(primerPulso==false){ //Estado inicial ambos leds apagados
    digitalWrite(52,LOW);
    digitalWrite(53,LOW);
  }

  if(valor==true && estadoAnterior==true){  //Apagado de sistema
    espera++; //Si se mantiene empieza a contar
    if(espera==apagar){
    primerPulso=false;
    salida=0;
    espera=0;
    digitalWrite(52,LOW);
    digitalWrite(53,LOW);
    }
  }

  if(valor==true && estadoAnterior==false){ //Cambio de led a prender o primer encendido
    salida=1-salida;
    primerPulso=true;
  }

  estadoAnterior=valor;

  if(salida==1 && primerPulso==true){ //Led pin52
    digitalWrite(52,HIGH);
    delay(100);
    digitalWrite(52,LOW);
    delay(100);

  }
  else if(salida==0 && primerPulso==true){ //Led pin53
     digitalWrite(53,HIGH);
     delay(100);
     digitalWrite(53,LOW);
     delay(100);
  }  
}