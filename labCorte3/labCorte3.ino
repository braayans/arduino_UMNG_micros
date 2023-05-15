//Laboratorio de tercer corte de micros. Brayan Romero y Julian Duarte
//Codigo para el arduino mega del lab


//variables para los pulsadores
const byte dato_SSD[10] = {0XC0, 0XF9, 0XA4, 0XB0, 0X99, 0X92, 0X82, 0XF8, 0X80, 0X98}; //Listado de numeros 0-9 ANODO COMUN SSD
const byte dato_SSD_error[10] = {0X80, 0Xf2, 0X48, 0X60, 0X33, 0X24, 0X04, 0XF0, 0X00, 0X20}; //SSD en caso de error
//volatile int conteo[2] = {0 , 0};  //Guarda el numero de veces que se presiona cada contador {unidad,decena}
volatile int unidad = 0;
volatile int decena = 0;

//Variables de tiempo 
volatile unsigned long int v_ini,v_ini2,v_ini3,v_ini4,v_act;


//Lista de recepcion de datos
char dato_recibido = "a";



//Contador de interrupciones
unsigned short int contador_interrupciones = 0;


//Pulsadores----------------------------------------------------
const int pulsador1 = 18;
const int pulsador2 = 19;
const int pulsador3 = 2;
const int pulsador4 = 3;
//--------------------------------------------------------------



//banderas de interrupciones------------------------------------
volatile bool bandera_1 = false;
volatile bool bandera_2 = false;
volatile bool bandera_3 = false;
volatile bool bandera_4 = false;
//--------------------------------------------------------------




//Variables para medir voltaje
float voltaje_leido;
int voltaje_real;
short int contador_voltaje = 0;
//--------------------------------------------------------------


//Variables sensor luz
const float Vin_luz = 4.99;
const float Vmin_luz = 0.09;
const float Vmax_luz = 4.89;
const float R1_luz = 9700;
float distancia_luz=(Vmax_luz-Vmin_luz)/100;
float Vout_luz;
int porcentaje_luz;
//----------------------------------------------------------------


//Variables de medidor resistencia
short int contador_resistencia = 0;
const float R1_resistencia = 9670;
const float Vin_resistencia = 4.99;
float Vout_resistencia = 0;
int R2_resistencia;
//----------------------------------------------------------------


//Variables de temperatura----------------------------------------
short int contador_temperatura = 0;
float valor_temperatura;
float Vout_temperatura;
int temperatura_celsius;
//-----------------------------------------------------------------




//Manejo de interrupciones-----------------------------------------
void pulsador_1_fun(){

  unidad++;
  if (unidad > 9){
    unidad = 0;
    decena++;
    if(decena > 9){
      unidad = 0;
      decena = 0;
    }
  }
  bandera_1 = true;
  detachInterrupt(digitalPinToInterrupt(pulsador1));
}

void pulsador_2_fun(){

  unidad++;
  if (unidad > 9){
    unidad = 0;
    decena++;
    if(decena > 9){
      unidad = 0;
      decena = 0;
    }
  }
  bandera_2 = true;
  detachInterrupt(digitalPinToInterrupt(pulsador2));
}

void pulsador_3_fun(){

  unidad++;
  if (unidad > 9){
    unidad = 0;
    decena++;
    if(decena > 9){
      unidad = 0;
      decena = 0;
    }
  }

  bandera_3 = true;
  detachInterrupt(digitalPinToInterrupt(pulsador3));
}

void pulsador_4_fun(){


  unidad++;
  if (unidad > 9){
    unidad = 0;
    decena++;
    if(decena > 9){
      unidad = 0;
      decena = 0;
    }
  }

  bandera_4 = true;
  detachInterrupt(digitalPinToInterrupt(pulsador4));
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
  Serial.begin(9600);
  DDRA = 0XFF; //Pulsador de unidades
  DDRC = 0XFF; //Pulsador de decenas


  pinMode(pulsador1,INPUT);
  pinMode(pulsador2,INPUT);
  pinMode(pulsador3,INPUT);
  pinMode(pulsador4,INPUT);

  attachInterrupt(digitalPinToInterrupt(pulsador1), pulsador_1_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador2), pulsador_2_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador3), pulsador_3_fun, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsador4), pulsador_4_fun, RISING);
 


  v_ini = millis();
  v_ini2 = millis();
  v_ini3 = millis();
  v_ini4 = millis();

  PORTA = dato_SSD[unidad];
  PORTC = dato_SSD[decena];
}




void loop() {


  calculo_voltaje();
  calculo_resistencia();
  calculo_luz();
  calculo_temperatura();



  if (Serial.available()){
    delay(5);
    char  dato_recibido_pru = Serial.read();
    Serial.print("Este es mi dato: ");
    Serial.println(dato_recibido_pru);

    if(dato_recibido_pru == 'm'){
      PORTC = 0XFF;
    }

  }




  //Pulsadores 
  // Primer pulsador

  if(bandera_1 == true){
    v_act = millis();
    if(v_act - v_ini > 800){
      PORTA = dato_SSD[unidad];
      PORTC = dato_SSD[decena];

      Serial.write(1);
      Serial.write(voltaje_real);
      Serial.write(temperatura_celsius);
      Serial.write(R2_resistencia);
      Serial.write(porcentaje_luz); 


      v_ini = millis();
      if(digitalRead(pulsador1) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador1), pulsador_1_fun, RISING);
        bandera_1 = false;
      }
    }
  } 

  //Segundo pulsador
 if(bandera_2 == true){
    v_act = millis();
    if(v_act - v_ini2 > 800){
      PORTA = dato_SSD[unidad];
      PORTC = dato_SSD[decena];

      Serial.write(2);
      Serial.write(voltaje_real);
      Serial.write(temperatura_celsius);
      Serial.write(R2_resistencia);
      Serial.write(porcentaje_luz); 

      v_ini2 = millis();
      if(digitalRead(pulsador2) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador2), pulsador_2_fun, RISING);
        bandera_2 = false;
      }
    }
  } 

  // Tercer pulsador
 if(bandera_3 == true){
    v_act = millis();
    if(v_act - v_ini3 > 800){
      PORTA = dato_SSD[unidad];
      PORTC = dato_SSD[decena];

      Serial.write(0x64);
      Serial.print(4);
      Serial.print(5);



      v_ini3 = millis();
      if(digitalRead(pulsador3) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador3), pulsador_3_fun, RISING);
        bandera_3 = false;
      }
    }
  } 


  // Cuarto pulsador 
 if(bandera_4 == true){
    v_act = millis();
    if(v_act - v_ini4 > 800){
      PORTA = dato_SSD[unidad];
      PORTC = dato_SSD[decena];

      Serial.write(4);
      Serial.write(voltaje_real);
      Serial.write(temperatura_celsius);
      Serial.write(R2_resistencia);
      Serial.write(porcentaje_luz); 

      v_ini4 = millis();
      if(digitalRead(pulsador4) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador4), pulsador_4_fun, RISING);
        bandera_4 = false;
      }
    }
  } 




  
}
