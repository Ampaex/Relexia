//          CÓDIGO FUENTE LIBRERÍA DE CONTROL DE RED RELEXIA
//                Autor: Antonio Vázquez Pérez

#include "Red.h"

//--------------------------------------CONEXION ALEXA------------------------------------------
void inicializaAlexa(fauxmoESP* conexionAlexa)
{

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    conexionAlexa->createServer(true); // not needed, this is the default value
    conexionAlexa->setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    conexionAlexa->enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
    conexionAlexa->addDevice("RELEXIA");
    conexionAlexa->addDevice("BRILLO");
    conexionAlexa->addDevice("AMBIENTE");
}

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

//------------------------------OBTENER EL ESTADO METEOROLÓGICO------------------------------------

DynamicJsonDocument obtenerMeteo(int dia)
{

    HTTPClient clienteHttp;
    DynamicJsonDocument documentoJson(15000);
    if (estadoWifi() == 1)
    {
        //Conecta con la api de la hora
        clienteHttp.begin(dia == 0 ? API_TIEMPO : API_PREVISION);

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
            Serial.print("Error obteniendo meteorología de la página. CÓDIGO HTTP:");
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
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, CONTRASENA);

    Serial.print("[WiFi]Conectando");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(".");
    }

    Serial.println("[WiFi-MODO ESTACIÓN]¡Conectado al punto de acceso!");
}


int estadoWifi()
{
    return WiFi.status() == WL_CONNECTED;
}