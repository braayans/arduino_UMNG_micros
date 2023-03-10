//BARS
const byte dato[10]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X98}; //Listado de numeros 0-9 ANODO COMUN SSD
int i=0,j=0,leer; //Super variables 

void mostrar(){      //Muestra valor en los puertos de salida
  PORTA=dato[i];     //A
  PORTC=dato[j];     //C
  delay(200);
}

void incremento(short a){   // "a" es el numero de saltos que se dan (1,2,3,4)
  i+=a;
  if(i>9){
    i%=10;      // Tambien se puede usar --> i-=10 mismo resultado
    j++;
    if(j>9) j=0;   // Se agrega i=0 si quiero dejar en 00 ej. 99 + 4 --> 00                    
    }  

  mostrar();

}

void setup(){
  DDRA=0XFF;   //PUERTO A COMO SALIDA
  DDRC=0XFF;    
  DDRL=0X00;   //ENTRADA
  mostrar();   
}

void loop() {
  leer=PINL&0X0F; //Lectura de puerto
  switch(leer){
    case 0:
      break;

    case 1:  // Primer pulsador  0x01
      incremento(1);
      break;

    case 2:  // segundo pulsador 0x02
      incremento(2);
      break;
    
    case 4:  // Tercer pulsador 0x04
      incremento(3);
      break;

    case 8:  //Cuarto pulsador  0x08
      incremento(4);
      break;

    default:
      break;

  }
}














/*void setup(){
  DDRC=0xFF;
}


void loop(){
  



  PORTC=0x80;
  delay(200);
  PORTC=0x00;
  delay(100);
}
*/