//          CÓDIGO FUENTE LIBRERÍA DE CONTROL DE PERIFÉRICOS RELEXIA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 20/03/2020


#include "Perifericos.h"



//Ésta función debe ir en la función setup() para inicializar la conexión wifi
void conectaWiFi(){
    Serial.begin(115200);
 
    WiFi.begin(SSID, CONTRASENA);
    
    if(DEBUG)Serial.print("[WiFi]Conectando");

    while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     if(DEBUG)Serial.println(".");
    }
 
    if(DEBUG)Serial.println("[WiFi]¡Conectado al punto de acceso!");

}

int compruebaWiFi(){
    return WiFi.status() == WL_CONNECTED;
}
