//          CABECERA DE CONTROL DE ESCENAS RELEXIA
//                Autor: Antonio Vázquez Pérez

#include "Perifericos.h"
#include "Red.h"

//Escenas
void escenaReloj(RtcDS1302<ThreeWire> rtc, CRGB* leds, int* escenario, unsigned long ultimaAct, fauxmoESP conexionAlexa);
void escenaTempHum(AHT10 myAHT10, CRGB* leds, int* escenario, unsigned long ultimaAct, fauxmoESP conexionAlexa);

//Control de escenas
void mostrarMeteo(CRGB* leds, DynamicJsonDocument meteo);
void efectoFundido(int paso, int velocidad, int apagadoEncendido); //Hace un efecto de fundido a apagado(0) o encendido(1)


bool esTiempo(unsigned long tiempoAnterior, unsigned long tiempoActual, unsigned int tiempoAEsperar);//Comprueba si ha pasado el tiempo que se indica


void administradorDeEscenas(int* escenario);