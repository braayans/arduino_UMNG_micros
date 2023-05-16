//Codigo comunicacion serial entre arduinos Brayan Romero Julian Duarte
// Codigo para arduino UNO


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>
Adafruit_SSD1306 display(128,64);




//------------------Variables millis-------------------------------
unsigned long int flanco_subida1, flanco_subida2, flanco_subida3, flanco_subida4, flanco_subida5;
unsigned long int v_ini_leds[4],v_act_leds;

//Contadores
short int contadores_pulsadores[5] = {0 ,0 ,0 ,0 ,0 };
bool  banderas_parpadeos[4] = {false,false,false,false};
bool cambios_leds[4] = {false,false,false,false};


//-----------------Leds--------------------------------------------
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;
//--------------------------------------------------------------


//Lectura de pulsacion
const int pulsador1 = 8;
const int pulsador2 = 9;
const int pulsador3 = 10;
const int pulsador4 = 11;
const int pulsador5 = 12;


short int lectura1 = 0;
short int lectura2 = 0;
short int lectura3 = 0;
short int lectura4 = 0;
short int lectura5 = 0;


short int lectura1_anterior = 0;
short int lectura2_anterior = 0;
short int lectura3_anterior = 0;
short int lectura4_anterior = 0;
short int lectura5_anterior = 0;
//-------------------------------------------------------------------

//Bandera para mostrar
bool vacio = true;

//Variables de recepcion---------------------------------------------
int sensores_recepcion[5] = {-1,0,0,0,0};    //bandera,voltaje,Temperatura,resistencia,luz
bool band1 = false;
bool band2 = false;
bool band3 = false;
bool band4 = false;
bool activado = false;
short int a = 0;
//------------------------------------------------------------------








//----------------Variables para medir voltaje--------------------
float voltaje_leido;
float voltaje_real;
//----------------------------------------------------------------


//---------------Variables de temperatura-------------------------
short int contador_temperatura = 0;
float valor_temperatura;
float Vout_temperatura;
float temperatura_celsius;
//-----------------------------------------------------------------


//--------------Variables de medidor resistencia------------------
short int contador_resistencia = 0;
const float R1_resistencia = 1000;
const float Vin_resistencia = 4.99;
float Vout_resistencia = 0;
float R2_resistencia;
//----------------------------------------------------------------


//--------------Variables sensor luz------------------------------
const float Vin_luz = 4.99;
const float Vmin_luz = 0.09;
const float Vmax_luz = 4.89;
const float R1_luz = 9700;
float distancia_luz=(Vmax_luz-Vmin_luz)/100;
float Vout_luz;
float porcentaje_luz;
//----------------------------------------------------------------


//-----------------Calculos de las variables en loop--------------
void calculo_voltaje(){
  voltaje_leido = (sensores_recepcion[1]* 5)/254.0;
  voltaje_real = voltaje_leido * (50/4.5);
}

void calculo_temperatura(){
  valor_temperatura = sensores_recepcion[2];
  Vout_temperatura = (valor_temperatura*5)/254.0;
  temperatura_celsius = Vout_temperatura*100;

}

void calculo_resistencia(){
  float valor_resistencia = sensores_recepcion[3];
  Vout_resistencia=valor_resistencia * (5 / 254.0);
  R2_resistencia = (Vout_resistencia * R1_resistencia) / (Vin_resistencia-Vout_resistencia);
}

void calculo_luz(){
  float valor_luz = sensores_recepcion[4];
  Vout_luz = valor_luz*( 5 / 254.0 );
  porcentaje_luz = 100 -( Vout_luz - Vmin_luz )/distancia_luz;
  if(porcentaje_luz >= 100.0) porcentaje_luz = 100;
  if(porcentaje_luz <= 0.0) porcentaje_luz = 0;
}




//------------------------------------------------------------------------

void mostrar(){


  if(vacio == true){
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Esperando...");
    }


  else{

    display.clearDisplay();

   if(band1 == true) {
      display.setTextColor(WHITE);      
    }else{
      display.setTextColor(BLACK);
   }
    display.setCursor(0,10);
    display.print("Voltaje: ");
    display.println(voltaje_real);



    if(band2 == true){
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
    display.cp437(true);
    display.setCursor(0,20);
    display.print("Temperatura: ");
    display.print(temperatura_celsius);
    display.write(167);
    display.println("");
    


   if(band3 == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
   }
   // display.setTextSize(1);
    display.setCursor(0,30);
    display.print("Resistencia: ");
    display.println(R2_resistencia);
    


    if(band4 == true) {
      display.setTextColor(WHITE);
    }else{
      display.setTextColor(BLACK);
    }
   // display.setTextSize(1);
    display.setCursor(0,40);
    display.print("Luz: ");
    display.print(porcentaje_luz);
    display.println(" % ");
  }
    
    display.display();





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

  if(a == 1){   

    if(banderas_parpadeos[0] == false){banderas_parpadeos[0] = true;}

    contadores_pulsadores[0]++;

    if(contadores_pulsadores[0] > 1){
      Serial.write(1);
      contadores_pulsadores[0] = 0;
    }
  }
  

  if (a == 2){

    if(banderas_parpadeos[1] == false) banderas_parpadeos[1] = true;

    contadores_pulsadores[1]++;      
    if(contadores_pulsadores[1] > 1){
      Serial.write(2);
      contadores_pulsadores[1] = 0;
    }        
  }

  if(a == 3){
    if(banderas_parpadeos[2] == false) banderas_parpadeos[2] = true;
    contadores_pulsadores[2]++;

    if(contadores_pulsadores[2] > 1){
      Serial.write(3);
      contadores_pulsadores[2] = 0;
    }
  }
    



  if (a == 4){
    if(banderas_parpadeos[3] == false) banderas_parpadeos[3] = true;
    contadores_pulsadores[3]++;

    if(contadores_pulsadores[3] > 1){
      Serial.write(4);
      contadores_pulsadores[3] = 0;
    }
  }
      

  if (a == 5){
    contadores_pulsadores[4]++;

    if(contadores_pulsadores[4] > 1){
      Serial.write(5);
      contadores_pulsadores[4] = 0;
    } 
  }
      

}








void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Iniciar pantalla
  display.clearDisplay();
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.dim(false);


  Serial.begin(9600);
  delay(1000);


  pinMode(LED1,OUTPUT);   
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);


  pinMode(pulsador1,INPUT);
  pinMode(pulsador2,INPUT);
  pinMode(pulsador3,INPUT);
  pinMode(pulsador4,INPUT);
  pinMode(pulsador5,INPUT);


  for(int m = 0; m < 4; m++){
    v_ini_leds[m] = millis();  
  }
}

void loop() {



  mostrar();

  //Lectura de pulsadores
  lectura1 = digitalRead(pulsador1);
  if (lectura1 == HIGH && lectura1_anterior == LOW && (millis() - flanco_subida1) > 50 )
  {  
    envio(1);
    flanco_subida1 = millis();

  }  
  lectura1_anterior = lectura1;


  lectura2 = digitalRead(pulsador2);
  if (lectura2 == HIGH && lectura2_anterior == LOW && (millis() - flanco_subida2) > 50 )
  {  
    envio(2);
    flanco_subida2 = millis();

  }  
  lectura2_anterior = lectura2;


  lectura3 = digitalRead(pulsador3);
  if (lectura3 == HIGH && lectura3_anterior == LOW && (millis() - flanco_subida3) > 50 )
  {  
    envio(3);
    flanco_subida3 = millis();

  }  
  lectura3_anterior = lectura3;

  lectura4 = digitalRead(pulsador4);
  if (lectura4 == HIGH && lectura4_anterior == LOW && (millis() - flanco_subida4) > 50 )
  {  
    envio(4);
    flanco_subida4 = millis();

  }  
  lectura4_anterior = lectura4;

  lectura5 = digitalRead(pulsador5);
  if (lectura5 == HIGH && lectura5_anterior == LOW && (millis() - flanco_subida5) > 50 )
  {  
    envio(5);
    flanco_subida5 = millis();

  }  
  lectura5_anterior = lectura5;


  while(Serial.available() > 0){
    delay(5);
    sensores_recepcion[a] = Serial.read();
     a++;
    if(a > 4) a = 0;
    
    activado = true;
  }

  a = 0;
  

  if(activado == true){
    vacio = false;
    switch(sensores_recepcion[0]){
      case 1:
        band1 = true;
        calculo_voltaje();
        break;
      case 2:
        band2 = true;
        calculo_temperatura();
        break;
      case 3:
        band3 = true;
        calculo_resistencia();
        break;
      case 4:
        band4 = true;
        calculo_luz();
        break;
      
      case 5:
        band1 = true;
        band2 = true;
        band3 = true;
        band4 = true;
        calculo_voltaje();
        calculo_temperatura();
        calculo_resistencia();
        calculo_luz();
        break;

      default:
        break;
    }

    activado = false;

  }

  parpadeo();
}




