//BARS
const byte dato[10] = {0XC0, 0XF9, 0XA4, 0XB0, 0X99, 0X92, 0X82, 0XF8, 0X80, 0X98}; //Listado de numeros 0-9 ANODO COMUN SSD
const byte serieLibre[6]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF};   //Siete segmentos estilo libre
const byte ledLibre[4]={0x08,0x04,0x02,0x01};
short x=26;  //estilo libre 10 segundos
int i = 0, j = 0, leer; //i j para mostrar y leer para tomar el valor de los pulsadores
short decre=0; //Cuenta cantidad de veces que se pulsa quinto pulsador

int conteo[4][2] = {{0, 0} , {0, 0}, {0, 0}, {0, 0}};  //Guarda el numero de veces que se presiona cada contador {unidad,decena}
byte leds=0x00; //Ver que LEDS se prenden


// Revisa que contador se pulsa y deja parpadeando el LED correspondiente
void parpadeo(){
  if(conteo[0][0]>0){
      leds=leds|0x01;
    }

  if(conteo[1][0]>0){
      leds=leds|0x02;
    }

  if(conteo[2][0]>0){
      leds=leds|0x04;
    }    
     
  if(conteo[3][0]>0){
      leds=leds|0x08;
    }      
 
  PORTB=leds;
  delay(100);
  PORTB=0X00;
  delay(100);
 
}


void parpadeoLibre(){

  PORTB=0x08;
  delay(1);

  for(int c=0;c<4;c++){
    PORTB=ledLibre[c];
    delay(50);
  }
  int c=4;
  while(c>=0){
    PORTB=ledLibre[c];
    delay(50);
    c--;
  }
}

void mostrar() {     //Muestra valor en los puertos de salida
  PORTA = dato[i];   //A unidades
  PORTC = dato[j];   //C decenas
  delay(20);
}

void pulsa(byte pulsado, short caso) { //Solo ejecuta la accion se se sueltan el o los pulsadores
  while (leer == PINL & 0X1F) {  
    delay(20);
    parpadeo();
  }
  if(caso==0 ||caso==1||caso==2||caso==3) incremento(caso); //Incrementa si solo se presiona uno de los primeros 4 pulsadores
 
  if(caso==4){   //Dentro de este caso se vigila si el quinto pulsador se presiona 4 veces
    decre++;
    if(decre > 3){
      decremento();
      decre=0;
      }
    }

  if(caso==5) libre(); //Se ejecuta la secuencia libre 
  
}

void decremento(){

  //-------Decremento de los pulsadores------------------------
  for(short m=0;m<4;m++){
    if((conteo[m][0]==conteo[m][1]) && (conteo[m][0]==0)){ //Caso de que sea 00 --> 00
      conteo[m][0]=0; //unidades
      conteo[m][1]=0;      
      }
    else if((conteo[m][0]==0) && (conteo[m][1]!=0)){ //Caso de que sea 10 20 o 30 --> 09 19 29
      conteo[m][0]=9; //unidades
      conteo[m][1]-= 1; //Decenas      
      }
    else{
      conteo[m][0]-= 1; //Unidades //Cualquier otro caso
      if(conteo[m][0]<0){
        conteo[m][1]-=1;
        }
    }
  }
  
  //--------Decremento del valor que se este mostrando en pantalla---------------
  if((i==j)&&(i==0)){
    i=0;
    j=0;
  }
  else if((i==0)&&(j!=0)){
    i=9;
    j-=1;
  }
  else{
    i-=1;
    if(i<0) j-=1;
  }

  mostrar();
}

void incremento(short a){  //Se aumenta en uno el valor de las veces que se ha oprimido cierto contador y lo muestra en pantalla
  i=conteo[a][0];
  j=conteo[a][1];
  i++;
  if(i>9){
    j=j+(i/10);
    i%=10;
  }
  if(j>9){
    j=0;
    i=0;  
  }
  conteo[a][0]=i;
  conteo[a][1]=j;
  mostrar();
  
  mostrar();
}

void libre(){
  //SERIE LIBRE
  
    for(int n=0;n<=10;n++)
    {
      if(n<10){
      PORTA=dato[n];
      PORTC=dato[0];
      delay(1000);
      parpadeoLibre();
      }
      else{
      PORTA=dato[0];
      PORTC=dato[1];
      delay(1000);
      parpadeoLibre();
      }
      
    }


  //REINCIO DE TODO EL PROGRAMA
  leds=0x00;
  for(int m;m<4;m++){
    conteo[m][0]=0;
    conteo[m][1]=0;
  }
  i=0;
  j=0;
  x=26;


  mostrar();
}

void setup() {
  DDRA = 0XFF; //SSD como salida puerto A unidades
  DDRC = 0XFF; //SSD como salida puerto C Decenas
  DDRB = 0xFF; //Leds como salidas
  DDRL = 0X00; //Pulsadores puerto L entrada
  mostrar();
}

void loop() {
  leer = PINL & 0X1F; //Lee los pulsadores
  switch (leer) {
    case 0:
      break;

    case 1:  // Primer pulsador  0x01
      pulsa(leer, 0);
      break;

    case 2:  // segundo pulsador 0x02
      pulsa(leer, 1);
      break;

    case 4:  // Tercer pulsador 0x04
      pulsa(leer, 2);
      break;

    case 8:  //Cuarto pulsador  0x08
      pulsa(leer, 3);
      break;

    case 16:
      pulsa(leer,4);
      break;
  
    default:
      pulsa(leer,5);
      break;

  }
  parpadeo();
}