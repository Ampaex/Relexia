//          CABECERA LIBRERÍA DE CONTROL DE REDES RELEXIA
//                Autor: Antonio Vázquez Pérez

#define API_FECHAYHORA "http://worldtimeapi.org/api/timezone/Europe/Madrid"
#define API_TIEMPO "https://api.openweathermap.org/data/2.5/weather?q=Zafra,es&appid="
#define API_PREVISION "https://api.openweathermap.org/data/2.5/onecall?lat=38.424646&lon=-6.417603&exclude=current,hourly,minutely&appid=&units=metric" 

//LIBRERÍAS
#include <HTTPClient.h>
#include <ArduinoJson.h>

//CONFIGURACIÓN WiFi
#include <WiFi.h>
#define SSID "vodafoneEFD0"
#define CONTRASENA "46K9QEV97SAYL7"

//INTERFAZ ALEXA

#include "fauxmoESP.h"
void inicializaAlexa(fauxmoESP* conexionAlexa);

//Devuelve StaticJsonDocument con la fecha y hora; NULL si falla
StaticJsonDocument<5000> obtenerFecha();

//@param dia 0 para el día de hoy y 1 para mañana
DynamicJsonDocument obtenerMeteo(int dia);

//WiFi
//Conecta con el wifi indicado en la configuración
void conectaWiFi();
//Devuelve 1 si la conexión wifi se encuentra activa
int estadoWifi();