#include <Adafruit_SSD1306.h>  // incluye la libreria de adafruit para el driver del display OLED
#include <Adafruit_GFX.h>  // incluye la libreria de adafruit para gestionar los efectos graficos de manera facil
#include <Fonts/FreeSerifBold24pt7b.h> // incluye una fuente disponible en el paquete de la libreria adafruit
#include <Fonts/FreeSansBold12pt7b.h> // incluye una fuente disponible en el paquete de la libreria adafruit
Adafruit_SSD1306 display(128, 64);  // declara la resolucion del display

int incremento=10; // valor del incremento en el contador
int limite=100; // el valor limite del contador para no desbordar la pantalla
int cont=-1*incremento; // valor inicial -el incremento para inicializar antes de contar
int pulsador=53; // pin en donde se conecta el pulsador
bool estPulActual=1; // variable donde se guarda el estado logico actual del pulsador
bool estPulAnterior=1; // variable donde se guarda el estado logico anterior del pulsador

void setup(){                
  
  pinMode(pulsador, INPUT_PULLUP); //el pin pulsador es asignado como entrada
  delay(100);  // se da una espera de 100ms para que el display inicie correctamente
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // inicia la comunicacion I2C con el display que tiene la direccion 0x3C
  display.setRotation(0);  // se escoje la orientacion del display puede ser 0, 1, 2 o 3
  display.setTextColor(WHITE);  // se le dice a la pantalla que el texto que se va a desplegar es en color blanco 
  display.dim(false); //dejamos el brillo en maximo

} 

void loop(){

  estPulActual=digitalRead(pulsador); //se lee el valor logico del pin de entraday se guarda en la variable estPulActual
  if((estPulAnterior==1 && estPulActual==0)|| cont==-1*incremento){ //valida si el pulsador ha cambiado de 1 a 0 ó si el contador esta en -1
    cont=cont+incremento; // suma 1 al valor de la variable contador
    if(cont>limite){ // si el contador supera el limite 
      cont=0; // devuelve el contador a 0
      }
    display.clearDisplay(); //limpia el buffer del display
    display.setFont(&FreeSansBold12pt7b); // selecciona la fuente basica por defecto 
    display.setTextSize(1);  // ajusta el tamaño de texto en el minimo valor
    display.setCursor(0, 18); //(x,y) ubica en la posicion (0,10) el cursor del display 
    display.print("Contador :"); //escribe "Contador:" en buffer del display    
    //display.display(); // muestra la informacion del buffer en la pantalla 
    display.setFont(&FreeSerifBold24pt7b); // cambia la fuente por la que incluimos al inicio del codigo 
    display.setTextSize(1); // ajusta el tamaño de texto de la fuente a 2
    display.setCursor(14, 60); //(x,y) ubica en la posicion (0,50) el cursor del display 
    display.print(cont); //escribe el valor en numeros de la variable contador en buffer del display    
    display.display(); // muestra la informacion del buffer en la pantalla 
    }
  delay(20); //pausa de 20 milisegundos
  estPulAnterior=estPulActual; //el valor booleano actual del contador ya fue procesado asi que se guarda en la variable estPulAnterior
}  // final de la funcion principal, salta al inicio del "void loop"

