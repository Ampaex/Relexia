//          CABECERA LIBRERÍA DE CONTROL DE PERIFÉRICOS RELEXIA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 7/06/2020

#include "fauxmoESP.h"

//CONFIGURACION MICRO SD
#include "SD.h"
#include "SPI.h"

//CONFIGURACIÓN PANTALLA
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#define PANTPIN 16
#define BRILLO 30     //0-255
#define NUMPIXELS 256 // Número de píxeles

//CONFIGURACIÓN RTC(RELOJ)
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#define RELPIN_CS 27
#define RELPIN_CLK 25 
#define RELPIN_DAT 26 

//CONFIGURACIÓN SENSOR PIR
#define BUTTON_PIN_BITMASK 0x4000       // 2^14 en hexadecimal, pin 14
#define PUEDE_DORMIR 0                  //Define si el dispositivo puede irse a dormir
#define TIEMPO_HASTA_SLEEP_MILLIS 20000 //Tiempo que transcurre hasta que se vuelve a dormir
#define WAKE_PIN GPIO_NUM_33            //Pin que queda a la escucha

//CONFIGURACIÓN BOTÓN
#define BOTONACCION 17

//===SD===
int inicializaSD();
void leeArchivo(fs::FS &fs, const char *path);

//===PANTALLA===
void inicializaPantalla(CRGB* leds);
int desplazaPixel(int posicion, int xOffs, int yOffs);                      //Devuelve la posición que ha de tener el pixel correspondiente desplazado en x e y
void pintarNUM(int num, CRGB color, CRGB* leds, int xOffs, int yOffs);      //Permite pintar un número pudiendo ser desplazado
void pintarTIEMPO(int selector, CRGB *leds, int xOffs, int yOffs);
void pintarVIENTO(int selector, CRGB *leds, int xOffs, int yOffs);
void pintarICONO(int num, CRGB color, CRGB *leds, int xOffs, int yOffs);
void borrarPixeles(CRGB* leds);                                             //Pone toda la pantalla en negro

//===RTC===
void inicializaReloj(RtcDS1302<ThreeWire> Rtc);
void cargaFecha(RtcDS1302<ThreeWire> Rtc);
RtcDateTime obtenerFechaRTC(RtcDS1302<ThreeWire> Rtc);
void imprimirFechaRTC(RtcDateTime dt);
void compruebaDormir(unsigned long millisUltimaAct, CRGB *leds); //Comprueba si se puede ir a dormir, si es así se pone a ello

//===TEMPERATURA Y HUMEDAD===
#include <AHT10.h>
#include <Wire.h>
void inicializaHumTemp(AHT10 myAHT10);
float obtenerTemperatura(AHT10 myAHT10);    //Precisión temperatura +- 0.3ºC
float obtenerHumedad(AHT10 myAHT10);        //Precisión humedad +- 2%

//===BOTONES===
