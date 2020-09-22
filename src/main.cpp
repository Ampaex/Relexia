//          CÓDIGO PRINCIPAL DISPOSITIVO RELEXA
//                Autor: Antonio Vázquez Pérez

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
int brillo = 30;
int colorAmbiente = 0x2020C0;

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
int escenario = 0; 
void administradorDeEscenas();
unsigned long millisUltimaLlamada = 0; //Para evitar rebote del botón

void setup() {
    if(DEBUG) Serial.begin(115200);

    //Para evitar deslumbrar, por la noche, se pone el brillo por defecto en bajo
    RtcDateTime fecha = obtenerFechaRTC(rtc);
    if(fecha.Hour() < 7 || fecha.Hour() > 22)
    {
        Serial.println("Brillo reducido");
        brillo = 4;
    }

    conectaWiFi();
    
    inicializaAlexa(&conexionAlexa);


    conexionAlexa.setState(1, true, brillo);    //Reportamos nuestro nivel de brillo de pantalla a alexa
    conexionAlexa.setState(2, false, 0);        //La pantalla ambiente por defecto desactivada

    conexionAlexa.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
        if (strcmp(device_name, "RELEXIA")==0) {
            Serial.println("RELEXIA activado ");
        } else if (strcmp(device_name, "BRILLO")==0) {
            if(state==false){
                FastLED.setBrightness(0);
                FastLED.show();
            }else{
                brillo = value;
                Serial.printf("Brillo de la pantalla al %d \n", brillo);
                FastLED.setBrightness(value);
                FastLED.show();
            }
            
        } else if (strcmp(device_name, "AMBIENTE")==0) {
            if(state == true){
                escenario = 3;
                if(value > 220 && value < 254)
                {
                    colorAmbiente = 0xF41E1E;   //Rojo
                }
                else
                {
                    colorAmbiente = 0x1515B0;   //Azul
                }
            }else{
                escenario = 0;
            }
            
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

    //Se asigna el pin encargado de atender a las interrupciones del sensor de presencia
    pinMode(WAKE_PIN, INPUT);
    esp_sleep_enable_ext0_wakeup(WAKE_PIN,1);

    inicializaPantalla(leds, brillo);

    ultimaActividad = millis();
}

void loop() {

    Serial.println(F("DEMO 1: read 12-bytes, show 255 if communication error is occurred"));
    Serial.print(F("Temperature: ")); Serial.print(obtenerTemperatura(myAHT10)); Serial.println(F(" +-0.3C")); //by default "AHT10_FORCE_READ_DATA"
    Serial.print(F("Humidity...: ")); Serial.print(obtenerHumedad(myAHT10));    Serial.println(F(" +-2%"));   //by default "AHT10_FORCE_READ_DATA"
    conexionAlexa.handle();
    switch (escenario)
    {
    case 0:
        escenaReloj(rtc, leds, &escenario, &ultimaActividad, &conexionAlexa, &brillo);
        break;
    case 1:
        escenaPrevision(leds, &escenario, &ultimaActividad, &conexionAlexa, &brillo);
        break;
    case 2:
        escenaTempHum(myAHT10, leds, &escenario, &ultimaActividad, &conexionAlexa, &brillo);
        break;
    case 3:
        escenaLuz(leds, &escenario, &conexionAlexa, &brillo, &colorAmbiente);
        break;
    
    default:
        break;
    }
    
}