//          CÓDIGO PRINCIPAL DISPOSITIVO RELEXA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 7/06/2020

//Activar para depurar
#define DEBUG 1

#include <Arduino.h>
#include "Perifericos.h"
#include "Red.h"
#include "Escenas.h"


//ALEXA

fauxmoESP conexionAlexa;

//PANTALLA
CRGB leds[NUMPIXELS];

//RTC
ThreeWire myWire(RELPIN_DAT, RELPIN_CLK, RELPIN_CS); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

//SENSOR PIR (WAKE FROM DEEP SLEEP)
unsigned long ultimaActividad;          //Guarda en que momento se realizó la última actividad (millis)

//SENSOR DE TEMPERATURA Y HUMEDAD
AHT10 myAHT10(AHT10_ADDRESS_0X38);
uint8_t readStatus = 0;

/*CONTROL DE ESCENAS
    0 -> Escena reloj
    1 -> Escena temperatura y humedad
*/
int escenario = 1; 
void administradorDeEscenas();
unsigned long millisUltimaLlamada = 0; //Para evitar rebote del botón

void setup() {
    if(DEBUG) Serial.begin(115200);

    conectaWiFi();
    
    //Control alexa
    conexionAlexa.createServer(true);
    Serial.println("Definiendo puerto del servidor");
    conexionAlexa.setPort(80);

    conexionAlexa.enable(true);

    conexionAlexa.addDevice("RELEXIA");
    conexionAlexa.addDevice("PANTALLA");

    conexionAlexa.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
        if (strcmp(device_name, "RELEXIA")==0) {
            Serial.println("RELEXIA activado ");
        } else if (strcmp(device_name, "PANTALLA")==0) {
            Serial.println("PANTALLA activado ");
        }
    });

    inicializaSD();
    inicializaReloj(rtc);
    inicializaHumTemp(myAHT10);
    
    if(obtenerFechaRTC(rtc).Year()<2020)
    {
        cargaFecha(rtc);
    }
    imprimirFechaRTC(obtenerFechaRTC(rtc));
    pinMode(BOTONACCION, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(BOTONACCION),administradorDeEscenas,FALLING);

    //Se asigna el pin encargado de atender a las interrupciones del sensor de presencia
    pinMode(WAKE_PIN, INPUT);
    esp_sleep_enable_ext0_wakeup(WAKE_PIN,1);

    inicializaPantalla(leds);

    ultimaActividad = millis();
}

void loop() {

    Serial.println(F("DEMO 1: read 12-bytes, show 255 if communication error is occurred"));
    Serial.print(F("Temperature: ")); Serial.print(obtenerTemperatura(myAHT10)); Serial.println(F(" +-0.3C")); //by default "AHT10_FORCE_READ_DATA"
    Serial.print(F("Humidity...: ")); Serial.print(obtenerHumedad(myAHT10));    Serial.println(F(" +-2%"));   //by default "AHT10_FORCE_READ_DATA"

    switch (escenario)
    {
    case 0:
        escenaReloj(rtc, leds, &escenario, ultimaActividad, conexionAlexa);
        break;
    case 1:
        escenaTempHum(myAHT10, leds, &escenario, ultimaActividad, conexionAlexa);
        break;
    
    default:
        break;
    }
    
}
