//BARS
//Librerias
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

//Variables globales puertos
byte leds = 0x00;
int leer;
bool vacio=true;
//---------------------------------------------------------------

//Variables sensor luz
const float Vin_luz=4.99;
const float Vmin_luz=0.09;
const float Vmax_luz=4.89;
const float R1_luz=9700;
float distancia_luz=(Vmax_luz-Vmin_luz)/100;
float porcentaje_luz;
float Vout_luz;
bool band2 = false;
short int contador2=0;
//----------------------------------------------------------------


//Variables de medidor resistencia
short int contador1=0;
bool band1=false;
const float R1_resistencia = 9670;
const float Vin_resistencia = 4.99;
float Vout_resistencia=0;
int R2_resistencia;
//----------------------------------------------------------------


//Variables de temperatura----------------------------------------
short int contador3 = 0;
bool band3=false;
float valor_temperatura;
float Vout_temperatura;
float temperatura_celsius;
//-----------------------------------------------------------------

//Calculos de las variables en loop
void calculo_resistencia(){
  float valor_resistencia=analogRead(A2);
  Vout_resistencia=valor_resistencia*0.00488;
  R2_resistencia = (Vout_resistencia*R1_resistencia)/(Vin_resistencia-Vout_resistencia);
}

void calculo_luz(){
  float valor_luz = analogRead(A3);
  Vout_luz = valor_luz*0.00488;
  porcentaje_luz = 100 -( Vout_luz - Vmin_luz )/distancia_luz;
  if(porcentaje_luz >= 100.0) porcentaje_luz = 100;
  if(porcentaje_luz <= 0.0) porcentaje_luz = 0;
  Serial.print(porcentaje_luz);
  Serial.println("%");
}

void calculo_temperatura(){
  valor_temperatura = analogRead(A4);
  Vout_temperatura = (valor_temperatura/1023)*5000;
  temperatura_celsius = Vout_temperatura/10;
}
//---------------------------------------------------------------


void parpadeo(){
  if (band1==true){
    leds = leds | 0x01; 
  }

  if (band2==true){
    leds = leds | 0x02; 
  }

  PORTB=leds; //Enciende leds
  delay(100);
  PORTB=0X00; //Apaga leds (parpadeo)
  delay(100);

}

//--------Banderas para activar variables---------
void resistencia(){
  band1 = true;
  vacio = false;
}

void luz(){
  band2 = true;
  vacio = false;
}


void temperatura(){
  band3 = true;
  vacio = false;
}
//----------------------------------------------------

void mostrar(){

  if (vacio==true){
    display.clearDisplay();
    //display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Esperando");
    display.display();
    delay(10);
  }

  if (vacio==false){
    
    display.clearDisplay();


   if(band1 == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Resistencia es: ");
    display.println(R2_resistencia);
    


    if(band2 == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
   // display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Luz: ");
    display.print(porcentaje_luz);
    display.println(" % ");

    
    if(band3 == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.cp437(true);
    display.setCursor(0,30);
    display.print("Temperatura: ");
    display.print(temperatura_celsius);
    display.write(167);
    display.println("");
    


    if(temperatura_celsius < 29) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.setCursor(120,48);
    display.print("A");

    display.display();
  }
}


void pulsa(int caso) { //Solo ejecuta la accion se se sueltan el o los pulsadores
  while (leer == PINL & 0X0F){  
    delay(20);
    parpadeo();
  }

  if(caso==0){
    contador1++;
    if (contador1>1){
      resistencia();
      contador1=0;
    }
  }

  if (caso==1){
    contador2++;
    if (contador2>1){
      luz();
      contador2=0;
    }
  }

  if (caso==2){
    contador3++;
    if (contador3>1){
      temperatura();
      contador3=0;
    }
  }
  
  mostrar();
}

void setup() {
  Serial.begin(9600);
  DDRL=0X00; //Puerto pulsador
  DDRB=0xFF; //LEDS 
  delay(100); //Para que inicie bien el OLED
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.dim(false);
}

void loop() {
  mostrar();

  calculo_resistencia();
  calculo_luz();
  calculo_temperatura();

  leer = PINL & 0X0F;
  switch (leer) {
    case 0:
      break;

    case 1:  // Primer pulsador  0x01
      pulsa(0);
      break;

    case 2:
      pulsa(1);
      break;

    case 4:
      pulsa(2);
      break;


    default:
      break;
   
  } 
  parpadeo();
}

