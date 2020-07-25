//          CÓDIGO PRINCIPAL DISPOSITIVO RELEXA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 7/06/2020

//Activar para depurar
#define DEBUG 1

#include <Arduino.h>
#include "Perifericos.h"
#include "Red.h"


//PANTALLA
//Adafruit_NeoPixel pixels(NUMPIXELS, PANTPIN, NEO_GRB + NEO_KHZ800);
CRGB leds[NUMPIXELS];

//RTC
ThreeWire myWire(RELPIN_DAT, RELPIN_CLK, RELPIN_CS); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

//SENSOR PIR (WAKE FROM DEEP SLEEP)
unsigned long ultimaActividad;          //Guarda en que momento se realizó la última actividad (millis)

//SENSOR DE TEMPERATURA Y HUMEDAD
AHT10 myAHT10(AHT10_ADDRESS_0X38);

uint8_t readStatus = 0;


void setup() {
    if(DEBUG) Serial.begin(115200);
    conectaWiFi();
    inicializaSD();
    inicializaReloj(rtc);
    inicializaHumTemp(myAHT10);

    if(obtenerFechaRTC(rtc).Year()<2020)
    {
        cargaFecha(rtc);
    }
    imprimirFechaRTC(obtenerFechaRTC(rtc));

    //Se asigna el pin encargado de atender a las interrupciones del sensor de presencia
    pinMode(WAKE_PIN, INPUT);
    esp_sleep_enable_ext0_wakeup(WAKE_PIN,1); //1 = High, 0 = Low

    //inicializaPantalla(pantalla);

    FastLED.addLeds<WS2812B, PANTPIN, GRB>(leds, NUMPIXELS);
    FastLED.setBrightness(BRILLO);

    ultimaActividad = millis();
}

void loop() {
    
    if (PUEDE_DORMIR && (millis() - ultimaActividad >= TIEMPO_HASTA_SLEEP_MILLIS))
    {
        Serial.println("YÉNDOSE A DORMIR");
        esp_deep_sleep_start();
    }

    Serial.println(F("DEMO 1: read 12-bytes, show 255 if communication error is occurred"));
    Serial.print(F("Temperature: ")); Serial.print(obtenerTemperatura(myAHT10)); Serial.println(F(" +-0.3C")); //by default "AHT10_FORCE_READ_DATA"
    Serial.print(F("Humidity...: ")); Serial.print(obtenerHumedad(myAHT10));    Serial.println(F(" +-2%"));   //by default "AHT10_FORCE_READ_DATA"

/*
    for (int i = 0; i< NUMPIXELS; i++)
    {
        leds[i]= CRGB::Black;
    }
    */
    borrarPixeles(leds);
    FastLED.show();
    Serial.println("Borrado");
    delay(3000);
    

    /*
    for (int i = 0; i< NUMPIXELS; i++)
    {
        leds[i]= CRGB::White;
        FastLED.show();
        delay(50);
    }*/

    int desl = 0;
    int num = 0;
    while (1)
    {
        borrarPixeles(leds);
        pintarNUM(num, CRGB::White, leds, desl, -desl);
        FastLED.show();
        delay(1000);
        desl++;
        num++;
    }
}