//          CÓDIGO PRINCIPAL DISPOSITIVO RELEXA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 20/03/2020


#include <Arduino.h>
#include "Perifericos.h"


void setup() {
    if(DEBUG)Serial.begin(115200);
    conectaWiFi();

    
}

void loop() {
    delay(2000);
}