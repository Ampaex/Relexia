//          CÓDIGO FUENTE LIBRERÍA DE CONTROL DE RED RELEXIA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 7/06/2020

#include "Red.h"


//--------------------------------------OBTENER FECHA Y HORA------------------------------------------

StaticJsonDocument<5000> obtenerFecha()
{

    HTTPClient clienteHttp;
    StaticJsonDocument<5000> documentoJson;
    if (estadoWifi() == 1)
    {
        //Conecta con la api de la hora
        clienteHttp.begin(API_FECHAYHORA);

        int httpRes = clienteHttp.GET();
        if (httpRes > 0) //Comprueba si la respuesta http es válida
        {
            String cuerpo = clienteHttp.getString();

            DeserializationError error = deserializeJson(documentoJson, cuerpo); //Deserializa a documentoJson

            if (error) //Comprueba si existe un error en la deserialización
            {
                Serial.print(F("Fallo en deserializeJson(): "));
                Serial.println(error.c_str());
            }
            //En este punto todo correcto
            
        }
        else
        {
            Serial.print("Error obteniendo fecha y hora de la página. CÓDIGO HTTP:");
            Serial.println(httpRes);
        }
    }
    else
    {
        conectaWiFi();
    }
    return documentoJson;
}

//----------------------------------------------WIFI--------------------------------------------------


void conectaWiFi()
{
    WiFi.begin(SSID, CONTRASENA);

    Serial.print("[WiFi]Conectando");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(".");
    }

    Serial.println("[WiFi]¡Conectado al punto de acceso!");
}


int estadoWifi()
{
    return WiFi.status() == WL_CONNECTED;
}