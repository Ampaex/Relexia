//          CABECERA LIBRERÍA DE CONTROL DE REDES RELEXIA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 7/06/2020
//       Última versión: 7/06/2020

#define API_FECHAYHORA "http://worldtimeapi.org/api/timezone/Europe/Madrid"

//LIBRERÍAS
#include <HTTPClient.h>
#include <ArduinoJson.h>


//Devuelve StaticJsonDocument con la fecha y hora; NULL si falla
StaticJsonDocument<5000> obtenerFecha();

//CONFIGURACIÓN WiFi
#include <WiFi.h>
#define SSID "vodafoneEFD0"
#define CONTRASENA "46K9QEV97SAYL7"

//WiFi
//Conecta con el wifi indicado en la configuración
void conectaWiFi();
//Devuelve 1 si la conexión wifi se encuentra activa
int estadoWifi();
