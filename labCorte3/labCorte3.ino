//Laboratorio de tercer corte de micros. Brayan Romero y Julian Duarte
//Codigo para el arduino mega del lab

//Pulsadores----------------------------------------------------
const int pulsador1 = 18;
const int pulsador2 = 19;
const int pulsador3 = 20;
const int pulsador4 = 21;
//--------------------------------------------------------------



//banderas de interrupciones------------------------------------
volatile bool bandera_1 = false;
volatile bool bandera_2 = false;
volatile bool bandera_3 = false;
volatile bool bandera_4 = false;
//--------------------------------------------------------------




//Variables para medir voltaje
float voltaje_leido;
volatile float voltaje_real;
short int contador_voltaje = 0;
//--------------------------------------------------------------


//Variables sensor luz
const float Vin_luz = 4.99;
const float Vmin_luz = 0.09;
const float Vmax_luz = 4.89;
const float R1_luz = 9700;
float distancia_luz=(Vmax_luz-Vmin_luz)/100;
float Vout_luz;
volatile float porcentaje_luz;
//----------------------------------------------------------------


//Variables de medidor resistencia
short int contador_resistencia = 0;
const float R1_resistencia = 9670;
const float Vin_resistencia = 4.99;
float Vout_resistencia = 0;
volatile int R2_resistencia;
//----------------------------------------------------------------


//Variables de temperatura----------------------------------------
short int contador_temperatura = 0;
float valor_temperatura;
float Vout_temperatura;
volatile float temperatura_celsius;
//-----------------------------------------------------------------

//Manejo de interrupciones-----------------------------------------
void pulsador_1_fun(){
 //Se envia el numero del pulsador y el dato del voltaje
  Serial.print(1);
  Serial.print(voltaje_real);

  detachInterrupt(digitalPinToInterrupt(pulsador1));
  bandera_1 = true;
}

void pulsador_2_fun(){
// Se envia el numero del pulsador y el dato de temperatura
  Serial.print(2);
  Serial.print(temperatura_celsius);

  detachInterrupt(digitalPinToInterrupt(pulsador2));
  bandera_2 = true;
}

void pulsador_3_fun(){
// Se envia el numero del pulsador y el dato de resistencia
  Serial.print(3);
  Serial.print(R2_resistencia);

  detachInterrupt(digitalPinToInterrupt(pulsado3));
  bandera_3 = true;
}

void pulsador_4_fun(){

  Serial.print(4);
  Serial.print(porcentaje_luz);

  detachInterrupt(digitalPinToInterrupt(pulsador4));
  bandera_4 = true;
}

void pulsador_5_fun(){

  Serial.print(5);


  detachInterrupt(digitalPinToInterrupt(pulsador5));
  bandera_5 = true;
}

//-------------------------------------------------------------------





//Calculos de las variables en loop-------------------------------------
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
}

void calculo_temperatura(){
  valor_temperatura = analogRead(A3);
  Vout_temperatura = (valor_temperatura*5)/1023.0;
  temperatura_celsius = Vout_temperatura*100;
}

//------------------------------------------------------------------------









//--------------------------SETUP Y LOOP----------------------------------------------
void setup() {
  pinMode(pulsador1,INPUT);
  pinMode(pulsador2,INPUT);
  pinMode(pulsador3,INPUT);
  pinMode(pulsador4,INPUT);

  attachInterrupt(digitalPinToInterrupt(pulsador1), pulsador_1_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador2), pulsador_2_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador3), pulsador_3_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador4), pulsador_4_fun, RISING);
  //attachInterrupt(digitalPinToInterrupt(pulsador5), boton, RISING); 


  v_ini = millis();
}




void loop() {
  calculo_voltaje();
  calculo_resistencia();
  calculo_luz();
  calculo_temperatura();












//Pulsadores 
// Primer pulsador
  if(bandera_1 == true){
    v_act = millis();
    if(v_act - v_ini > 50){
      //Colocar envio de la info de voltaje al arduino UNO
      v_ini = millis();
      if(digitalRead(pulsador1) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador1), pulsador_1_fun, RISING);
        bandera_1 = false;
    }
  } 

//Segundo pulsador
 if(bandera_2 == true){
    v_act = millis();
    if(v_act - v_ini > 50){
      //Colocar envio de la info de resistencia al arduino UNO
      v_ini = millis();
      if(digitalRead(pulsador2) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador2), pulsador_2_fun, RISING);
        bandera_2 = false;
    }
  } 

// Tercer pulsador
 if(bandera_3 == true){
    v_act = millis();
    if(v_act - v_ini > 50){
      //Colocar envio de la info de voltaje al arduino UNO
      v_ini = millis();
      if(digitalRead(pulsador3) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador3), pulsador_3_fun, RISING);
        bandera_3 = false;
    }
  } 


// Cuarto pulsador 
 if(bandera_4 == true){
    v_act = millis();
    if(v_act - v_ini > 50){
      //Colocar envio de la info de voltaje al arduino UNO
      v_ini = millis();
      if(digitalRead(pulsador4) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador4), pulsador_4_fun, RISING);
        bandera_4 = false;
    }
  } 


  



}
