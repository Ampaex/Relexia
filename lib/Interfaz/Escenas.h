//          CABECERA DE CONTROL DE ESCENAS RELEXIA
//                Autor: Antonio Vázquez Pérez

#include "Perifericos.h"
#include "Red.h"

//Escenas
void escenaReloj(RtcDS1302<ThreeWire> rtc, CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo);
void escenaTempHum(AHT10 myAHT10, CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo);
void escenaLuz(CRGB* leds, int* escenario, fauxmoESP* conexionAlexa, int* brillo, int* color);
void escenaPrevision(CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo);

//Control de escenas
void mostrarMeteo(CRGB* leds, DynamicJsonDocument meteo);
void efectoFundido(int velocidad, int apagadoEncendido, int brillo); //Hace un efecto de fundido a apagado(0) o encendido(1)


bool esTiempo(unsigned long tiempoAnterior, unsigned long tiempoActual, unsigned int tiempoAEsperar);//Comprueba si ha pasado el tiempo que se indica


void administradorDeEscenas(int* escenario, unsigned long* ultimaActividad);

