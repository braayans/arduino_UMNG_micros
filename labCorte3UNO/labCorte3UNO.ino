//Codigo comunicacion serial entre arduinos Brayan Romero Julian Duarte
// Codigo para arduino UNO


#include<Wire.h>
#include <Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>


#define anchoPantalla 128
#define altoPantalla 64
#define oledReset -1
Adafruit_SSD1306 display(anchoPantalla,altoPantalla);




//Variables millis
unsigned long int v_ini, v_act;
unsigned long int v_ini_leds[4],v_act_leds;

//Contadores
short int contadores_pulsadores[5] = {0 ,0 ,0 ,0 ,0 };
bool banderas_parpadeos[4] = {false,false,false,false};
bool cambios_leds[4] = {false,false,false,false};


//Leds----------------------------------------------------
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;
//--------------------------------------------------------------


//Lectura de pulsacion
int pulsacion;
//-------------------------------------------------------------------

//Bandera para mostrar
bool vacio = true;

//Variables de recepcion---------------------------------------------
int sensores_recepcion[5] = {-1,-1,-1,-1,-1};
bool  banderas_recepcion[4] = {false,false,false,false};
bool activado = false;
short int a = 0;
//------------------------------------------------------------------

void mostrar(){
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

   if(banderas_recepcion[0] == true) {
      display.setTextColor(WHITE);      
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,10);
    display.print("Voltaje: ");
    display.println(sensores_recepcion[1]);



    if(banderas_recepcion[1] == true){
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.cp437(true);
    display.setCursor(0,20);
    display.print("Temperatura: ");
    display.print(sensores_recepcion[2]);
    display.write(167);
    display.println("");
    


   if(banderas_recepcion[2] == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,30);
    display.print("Resistencia es: ");
    display.println(sensores_recepcion[3]);
    


    if(banderas_recepcion[3] == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
   // display.setTextSize(1);
    display.setCursor(0,40);
    display.print("Luz: ");
    display.print(sensores_recepcion[4]);
    display.println(" % ");
  }
    






}

void parpadeo(){
  v_act_leds = millis();

  if(banderas_parpadeos[0] == true){
    v_act_leds = millis();
    if(v_act_leds - v_ini_leds[0] > 500){
      cambios_leds[0] = !cambios_leds[0];
      digitalWrite(LED1,cambios_leds[0]);
      v_ini_leds[0] = millis();
    }
  } 

  if(banderas_parpadeos[1] == true){
    v_act_leds = millis();
    if(v_act_leds - v_ini_leds[1] > 500){
      cambios_leds[1] = !cambios_leds[1];
      digitalWrite(LED2,cambios_leds[1]);
      v_ini_leds[1] = millis();
    }
  } 


  if(banderas_parpadeos[2] == true){
    v_act_leds = millis();
    if(v_act_leds - v_ini_leds[2] > 500){
      cambios_leds[2] = !cambios_leds[2];
      digitalWrite(LED3,cambios_leds[2]);
      v_ini_leds[2] = millis();
    }
  } 


  if(banderas_parpadeos[3] == true)
  {
    if(v_act_leds - v_ini_leds[3] > 500){
      cambios_leds[3] = !cambios_leds[3];
      digitalWrite(LED4,cambios_leds[3]);
      v_ini_leds[3] = millis();
    }
  } 


}


void envio(int a){
  while (pulsacion == PINB & 0X1F){  
    delay(20);
    mostrar();
    parpadeo();
  }

  switch(a){
    case 0:
      break;

    case 1:
      if(banderas_parpadeos[0] == false) banderas_parpadeos[0] = true;

      contadores_pulsadores[0]++;
      if(contadores_pulsadores[0] > 1){
        Serial.print(1);
        contadores_pulsadores[0] = 0;
        }
      break;

    case 2:
      if(banderas_parpadeos[1] == false) banderas_parpadeos[1] = true;

      contadores_pulsadores[1]++;      
      if(contadores_pulsadores[1] > 1){
        Serial.print(2);
        contadores_pulsadores[1] = 0;
        }           
      break;

    case 3:
      if(banderas_parpadeos[2] == false) banderas_parpadeos[2] = true;
      contadores_pulsadores[2]++;      
      if(contadores_pulsadores[2] > 1){
        Serial.print(3);
        contadores_pulsadores[2] = 0;
        }      
      break;

    case 4:
      if(banderas_parpadeos[3] == false) banderas_parpadeos[3] = true;
      contadores_pulsadores[3]++;      
      if(contadores_pulsadores[3] > 1){
        Serial.print(4);
        contadores_pulsadores[3] = 0;
        }      
      break;

    case 5:
      //--
      contadores_pulsadores[4]++;      
      if(contadores_pulsadores[4] > 1){
        Serial.print(5);
        contadores_pulsadores[4] = 0;
        }      
      break;

    default:
      break;

  }


}







void setup() {
  Serial.begin(9600);
  DDRB = 0X00;

  pinMode(LED1,OUTPUT);   
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);



  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Iniciar pantalla
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.dim(false);


  v_ini = millis();
  for(int m = 0; m < 4; m++){
    v_ini_leds[m] = millis();  
  }
}

void loop() {

  mostrar();
  while(Serial.available() > 0){
    delay(5);
    sensores_recepcion[a] = Serial.read();
    a++;
    activado = true;
  }
  a = 0;

  if(activado == true){
    vacio = false;
    switch(sensores_recepcion[0]){
      case 1:
        banderas_recepcion[0] = true;
        break;
      case 2:
        banderas_recepcion[1] = true;
        break;
      case 3:
        banderas_recepcion[2] = true;
        break;
      case 4:
        banderas_recepcion[3] = true;
        break;
      
      default:
        break;
    }

    activado = false;

  }




  pulsacion = PINB & 0x0F;  

  switch(pulsacion){
    case 0:
      break;
    
    case 1:

      envio(1);
      break;

    case 2:
      envio(2);
      break;

    case 4:
      envio(3);
      break;

    case 8:
      envio(4);
      break;

    case 16:
      envio(5);
      break;

    default:
      break;

  }  
 

  parpadeo();
}
