// BARS JADS
//Librerias de pantalla OLED, Nombre de libreria --> ADAFRUIT
#include<Wire.h>
#include <Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include <Fonts/FreeSerifBold24pt7b.h> 
#include <Fonts/FreeSansBold12pt7b.h>

//Atributos de pantalla------------------------------------------
#define anchoPantalla 128
#define altoPantalla 64
#define oledReset -1
Adafruit_SSD1306 display(anchoPantalla,altoPantalla);
//---------------------------------------------------------------

//Variables globales puerto LEDS y SSD
const byte dato[10] = {0XC0, 0XF9, 0XA4, 0XB0, 0X99, 0X92, 0X82, 0XF8, 0X80, 0X98}; //Listado de numeros 0-9 ANODO COMUN SSD
int conteo[4][3] = {{0, 0, 0} , {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};  //Voltaje,Temperatura,Resistencia,luz
int i = 0, j = 0, k = 0;
byte leds = 0x00;
int leer;
bool vacio=true;
//---------------------------------------------------------------


//Variables para medir voltaje
float voltaje_leido;
float voltaje_real;
bool band_voltaje = false;
short int contador_voltaje = 0;
//--------------------------------------------------------------


//Variables sensor luz
const float Vin_luz = 4.99;
const float Vmin_luz = 0.09;
const float Vmax_luz = 4.89;
const float R1_luz = 9700;
float distancia_luz=(Vmax_luz-Vmin_luz)/100;
float porcentaje_luz;
float Vout_luz;
bool band_luz = false;
short int contador_luz = 0;
//----------------------------------------------------------------


//Variables de medidor resistencia
short int contador_resistencia = 0;
bool band_resistencia = false;
const float R1_resistencia = 9670;
const float Vin_resistencia = 4.99;
float Vout_resistencia = 0;
int R2_resistencia;
//----------------------------------------------------------------


//Variables de temperatura----------------------------------------
short int contador_temperatura = 0;
bool band_temperatura = false;
float valor_temperatura;
float Vout_temperatura;
float temperatura_celsius;
//-----------------------------------------------------------------


//Variable de quinto pulsador--------------------------------------
short int contador_reinicio = 0;
//-----------------------------------------------------------------


//Variables de sexto pulsador--------------------------------------
short int contador_variables = 0;
//-----------------------------------------------------------------



//Calculos de las variables en loop
void calculo_voltaje(){
  voltaje_leido = (analogRead(A0)* 5)/1023.0;
  voltaje_real = voltaje_leido * (50/5);
}

void calculo_resistencia(){
  float valor_resistencia = analogRead(A1);
  Vout_resistencia=valor_resistencia * 0.00488;
  R2_resistencia = (Vout_resistencia * R1_resistencia) / (Vin_resistencia-Vout_resistencia);
}

void calculo_luz(){
  float valor_luz = analogRead(A2);
  Vout_luz = valor_luz*0.00488;
  porcentaje_luz = 100 -( Vout_luz - Vmin_luz )/distancia_luz;
  if(porcentaje_luz >= 100.0) porcentaje_luz = 100;
  if(porcentaje_luz <= 0.0) porcentaje_luz = 0;
  Serial.print(porcentaje_luz);
  Serial.println("%");
}

void calculo_temperatura(){
  valor_temperatura = analogRead(A3);
  Vout_temperatura = (valor_temperatura*5)/1023.0;
  temperatura_celsius = Vout_temperatura*100;
}
//---------------------------------------------------------------


void parpadeo(){
  if (band_voltaje == true){
    leds = leds | 0x01; 
  }

  if (band_resistencia == true){
    leds = leds | 0x02; 
  }

  if (band_luz == true){
    leds = leds | 0x04; 
  }

  if (band_temperatura == true){
    leds = leds | 0x08; 
  }

  PORTB = leds; //Enciende leds
  delay(100);
  PORTB = 0X00; //Apaga leds (parpadeo)
  delay(100);

}


//--------Banderas para activar variables------------------------------------------

void voltaje(){
  band_voltaje = true;
  vacio = false;
}

void resistencia(){
  band_resistencia = true;
  vacio = false;
}

void luz(){
  band_luz = true;
  vacio = false;
}


void temperatura(){
  band_temperatura = true;
  vacio = false;
}
//---------------------------------------------------------------------------------


//Acciones de quinto y sexto pulsdor
void reinicio(){              //Reiniciar programa con quinto pulsador


  band_voltaje = false;
  band_resistencia = false;
  band_temperatura = false;
  band_luz = false;

  vacio = true;




  for(int m = 0; m < 4; m++){
    conteo[m][0] = 0;
    conteo[m][1] = 0;
    conteo[m][2] = 0;

    i = 0;
    j = 0;
  }
  leds = 0x00;


  mostrar();
}

void mostrar_variables(){    //Mostrar todas las variables durante un segundo

  PORTA = 0X88;

  display.clearDisplay();

   
  display.setTextColor(WHITE);      
  display.setCursor(0,10);
  display.print("Voltaje: ");
  display.println(voltaje_real);

    
  display.setCursor(0,20);
  display.print("Resistencia es: ");
  display.println(R2_resistencia);
    

  display.setCursor(0,30);
  display.print("Luz: ");
  display.print(porcentaje_luz);
  display.println(" % ");

    
  display.cp437(true);
  display.setCursor(0,40);
  display.print("Temperatura: ");
  display.print(temperatura_celsius);
  display.write(167);
  display.println("");
    


  if(temperatura_celsius > 29) {
    display.setTextColor(WHITE);
  }else{
    display.setTextColor(BLACK);
  }
  display.setCursor(120,48);
  display.print("A");

  display.display();
  delay(1000);
}


//--------------------------------------------------------------------------------------

void mostrar(){                       //Mostrar OLED y contador de SSD

  if (vacio == true){
    display.clearDisplay();
    //display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Esperando");
    display.display();
    delay(10);
  }
  else if (vacio == false){
    
    display.clearDisplay();

   if(band_voltaje == true) {
      display.setTextColor(WHITE);      
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Voltaje: ");
    display.println(voltaje_real);




   if(band_resistencia == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Resistencia es: ");
    display.println(R2_resistencia);
    


    if(band_luz == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
   // display.setTextSize(1);
    display.setCursor(0,30);
    display.print("Luz: ");
    display.print(porcentaje_luz);
    display.println(" % ");

    
    if(band_temperatura == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.cp437(true);
    display.setCursor(0,40);
    display.print("Temperatura: ");
    display.print(temperatura_celsius);
    display.write(167);
    display.println("");
    


    if(temperatura_celsius > 29) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.setCursor(120,48);
    display.print("A");

    display.display();
  }
  
  mostrar_SSD();
}


void incremento_SSD(int a){           //Incremento SSD

  i=conteo[a][0]; //Unidades 
  j=conteo[a][1]; //Decenas
  k=conteo[a][2]; //Centenas
  i++;
  if(i>9){
    j=j+(i/10);
    i%=10;
  }
  if(j>9){
    k++;    
    j=0;
    i=0;  
  }
  conteo[a][0]=i;

  mostrar_SSD();
}



void mostrar_SSD(){
  PORTA = dato[i];   //A unidades
  delay(20);
}


void pulsa(int caso) { //Solo ejecuta la accion se se sueltan el o los pulsadores
  while (leer == PINL & 0X3B){  
    delay(20);
    mostrar();
    parpadeo();
  }


  if(caso == 0){     //En caso de presionar pulsador 1 
    contador_voltaje++;
    contador_luz = 0;
    contador_temperatura = 0;
    contador_resistencia = 0;
    if (contador_voltaje >= 2){
      voltaje();
      incremento_SSD(0);
      contador_voltaje = 0;
    }
  }



  if(caso == 2){      //En caso de presionar pulsadores 1 y 4
    contador_resistencia++;
    contador_luz = 0;
    contador_temperatura = 0;
    contador_voltaje = 0;
    if (contador_resistencia >= 3){
      resistencia();
          incremento_SSD(2);
      contador_resistencia = 0;
    }
  }


  if (caso == 3){
    contador_luz++;
    contador_resistencia = 0; 
    contador_temperatura = 0;
    contador_voltaje = 0;
    if (contador_luz >= 4){
      luz();
      incremento_SSD(3);
      contador_luz = 0;
    }
  }


  if (caso == 1){    //Caso temperatura
    contador_temperatura++;
    contador_resistencia = 0;
    contador_luz = 0;
    contador_voltaje = 0;
    if (contador_temperatura >= 2){
      temperatura();
      incremento_SSD(1);
      contador_temperatura = 0;
    }
  }


  if (caso == 4){    //Caso reinicio
    contador_reinicio++;
    contador_temperatura = 0;
    contador_resistencia = 0;
    contador_luz = 0;
    contador_voltaje = 0;
    if (contador_reinicio >= 2){
      reinicio();
      contador_reinicio = 0;
    }
  }



  if (caso == 5){    //Caso mostrar variables
    contador_variables++;
    contador_reinicio = 0;
    contador_temperatura = 0;
    contador_resistencia = 0;
    contador_luz = 0;
    contador_voltaje = 0;
    if (contador_variables >= 2){
      mostrar_variables();
      contador_variables = 0;
    }
  }



  
  mostrar();
}

void setup() {
  Serial.begin(9600);
  DDRA = 0XFF; //SSD
  DDRL = 0X00; //Puerto pulsador
  DDRB = 0xFF; //LEDS 
  delay(100); //Para que inicie bien el OLED
  
  //Configurar pantalla OLES
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Iniciar pantalla
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.dim(false);
}

void loop() {
  mostrar();  //Mostrar pantalla

  //Calculos de variables para que se actualicen 
  calculo_voltaje(); 
  calculo_resistencia();
  calculo_temperatura(); 
  calculo_luz();

  leer = PINL & 0X3B;
  switch (leer) {
    case 0:  //Caso no pasa nada
      break;

    case 1:  // Solo primer pulsador voltaje
      pulsa(0);
      break;

    case 3:  // Se presionan pulsadores uno y dos caso temperatura
      pulsa(1);
      break;

    case 9:   // Pulsadores 1 y 4 para medir resistencia
      pulsa(2);
      break;

    case 10:   // Pulsadores 2 y 4 para medir luz
      pulsa(3);
      break;


    case 16: //Quinto pulsador
      pulsa(4);
      break;
    
    case 32: //Sexto pulsador
      pulsa(5);
      break;

    default:
      break;
   
  } 
  parpadeo();
}

