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
unsigned long int flanco_subida;


//Lista de recepcion de datos
int dato_recibido = 0;



//Contador de interrupciones
unsigned short int contador_interrupciones = 0;


//Pulsadores----------------------------------------------------
const int pulsador1 = 18;
const int pulsador2 = 19;
const int pulsador3 = 2;
const int pulsador4 = 3;
const int pulsador5 = 10;
int leer = 0;
int lectura_anterior = 0;
//--------------------------------------------------------------



//banderas de interrupciones------------------------------------
volatile bool bandera_1 = false;
volatile bool bandera_2 = false;
volatile bool bandera_3 = false;
volatile bool bandera_4 = false;
//--------------------------------------------------------------




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











//--------------------------SETUP Y LOOP----------------------------------------------
void setup() {
  Serial.begin(9600);
  delay(1000);
  DDRA = 0XFF; //Pulsador de unidades
  DDRC = 0XFF; //Pulsador de decenas
  DDRB = 0x00; //Para el ultimo pulsador


  pinMode(pulsador1,INPUT);
  pinMode(pulsador2,INPUT);
  pinMode(pulsador3,INPUT);
  pinMode(pulsador4,INPUT);
  pinMode(pulsador5,INPUT);

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

  int voltaje_analogo = analogRead(A0);
  voltaje_analogo = voltaje_analogo >> 2;

  int temperatura_analogo = analogRead(A1);
  temperatura_analogo = temperatura_analogo >> 2;

  int resistencia_analogo = analogRead(A2);
  resistencia_analogo = resistencia_analogo >> 2;

  int luz_analogo = analogRead(A3);
  luz_analogo = luz_analogo >> 2;






  leer = digitalRead(pulsador5);
  if (leer == HIGH && lectura_anterior == LOW && (millis() - flanco_subida) > 50 )
  {  
    Serial.write(5);
    Serial.write(voltaje_analogo);
    Serial.write(temperatura_analogo);
    Serial.write(resistencia_analogo);
    Serial.write(luz_analogo);   

    flanco_subida = millis();

  }  
  lectura_anterior = leer;




  if (Serial.available()){

    delay(5);
    dato_recibido = Serial.read();

    if(dato_recibido == 1){
      Serial.write(1);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo);      
    }

    if(dato_recibido == 2){
      Serial.write(2);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo);      
    }

    if(dato_recibido == 3){
      Serial.write(3);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo);      
    }

    if(dato_recibido == 4){
      Serial.write(4);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo);      
    }

    if(dato_recibido == 5){
      Serial.write(5);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo);      
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
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo); 


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
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo); 

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

      Serial.write(3);
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo); 


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
      Serial.write(voltaje_analogo);
      Serial.write(temperatura_analogo);
      Serial.write(resistencia_analogo);
      Serial.write(luz_analogo); 

      v_ini4 = millis();
      if(digitalRead(pulsador4) == 0){
        attachInterrupt(digitalPinToInterrupt(pulsador4), pulsador_4_fun, RISING);
        bandera_4 = false;
      }
    }
  } 






  
}
