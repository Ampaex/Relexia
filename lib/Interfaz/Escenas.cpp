//          CÓDIGO FUENTE DE CONTROL DE ESCENAS RELEXIA
//                Autor: Antonio Vázquez Pérez

#include "Escenas.h"

void escenaReloj(RtcDS1302<ThreeWire> rtc, CRGB* leds, int* escenario, unsigned long ultimaAct, fauxmoESP conexionAlexa)
{
    RtcDateTime fechaAnterior = obtenerFechaRTC(rtc);
    RtcDateTime fechaActual;
    DynamicJsonDocument meteo(5000);//15000
    FastLED.setBrightness(0);
    meteo = obtenerMeteo();

    //Pinta la fecha que acabamos de leer
    borrarPixeles(leds);
    pintarNUM(fechaAnterior.Hour() / 10, CRGB::White, leds, 4, -2);
    pintarNUM(fechaAnterior.Hour() % 10, CRGB::White, leds, 9, -2);
    pintarNUM(fechaAnterior.Minute() / 10, CRGB::White, leds, 4, -9);
    pintarNUM(fechaAnterior.Minute() % 10, CRGB::White, leds, 9, -9);

    mostrarMeteo(leds, meteo);
    FastLED.show();
    efectoFundido(1, 400 / BRILLO, 1);

    while (*escenario == 0)
    {
        fechaActual = obtenerFechaRTC(rtc);

        if (fechaActual.Minute() != fechaAnterior.Minute() || fechaActual.Hour() != fechaAnterior.Hour())
        {
            fechaAnterior = fechaActual;
            pintarNUM(10, CRGB::Black, leds, 4, -2);
            pintarNUM(10, CRGB::Black, leds, 9, -2);
            pintarNUM(10, CRGB::Black, leds, 4, -9);
            pintarNUM(10, CRGB::Black, leds, 9, -9);

            pintarNUM(fechaActual.Hour() / 10, CRGB::White, leds, 4, -2);
            pintarNUM(fechaActual.Hour() % 10, CRGB::White, leds, 9, -2);
            pintarNUM(fechaActual.Minute() / 10, CRGB::White, leds, 4, -9);
            pintarNUM(fechaActual.Minute() % 10, CRGB::White, leds, 9, -9);
            FastLED.show();
        }
        Serial.println("Actualizada hora");

        //Espera 6s mientras comprueba
        unsigned long millisNow= millis();
        unsigned long millisNow2 = millis();
        while ((millis() - millisNow) <= 6000 && *escenario == 0)
        {
            compruebaDormir(ultimaAct, leds);
            administradorDeEscenas(escenario);
            if (esTiempo(millisNow2, millis(), 2000))
            {
                Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
                Serial.println("Comprueba alexa");
                //Se comprueban las conexiones con alexa
                if (estadoWifi() == 0)
                {
                    conectaWiFi();
                }
                millisNow2 = millis();
                conexionAlexa.handle();
            }
        }
    }
}

void escenaTempHum(AHT10 myAHT10, CRGB* leds, int* escenario, unsigned long ultimaAct, fauxmoESP conexionAlexa)
{
    float temperatura = obtenerTemperatura(myAHT10);
    float humedad = obtenerHumedad(myAHT10);
    FastLED.setBrightness(0);
    
    borrarPixeles(leds);
    //Representar temperatura
    pintarNUM((int)temperatura / 10, 0xBD0707, leds, 1, -2);
    pintarNUM((int)temperatura % 10, 0xBD0707, leds, 5, -2);
    leds[desplazaPixel(0, 9, -6)] = 0xBD0707; //Punto
    pintarNUM((int)(temperatura*10) % 10, 0xBD0707, leds, 11, -2);
    leds[desplazaPixel(0, 15, -2)] = 0xBD0707; //Cerito

    //Representar humedad
    pintarNUM((int)humedad / 10, 0x00A2E2, leds, 3, -9);
    pintarNUM((int)humedad % 10, 0x00A2E2, leds, 7, -9);
    pintarICONO(0, 0x00A2E2, leds, 11, -9);

    efectoFundido(1, 400/BRILLO, 1);

    //Comprueba
    unsigned long millisNow = millis();
    while(*escenario == 1)
    {
        compruebaDormir(ultimaAct, leds);
        administradorDeEscenas(escenario);
        if (esTiempo(millisNow, millis(), 2000))
        {
            Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
            
            //Se comprueban las conexiones con alexa
            if (estadoWifi() == 0)
            {
                conectaWiFi();
            }
            millisNow = millis();
            conexionAlexa.handle();
        }
    };

}

//Dada una meteorología la muestra en pantalla clasificada correctamente
void mostrarMeteo(CRGB* leds, DynamicJsonDocument meteo)
{
    int weatherCode = meteo["weather"][0]["id"];
    float windSpeed = meteo["wind"]["speed"];
    String descripcion = meteo["weather"][0]["description"];
    Serial.print("Tiempo: ");
    Serial.print(weatherCode);
    Serial.println(descripcion);
    Serial.print("Velocidad del viento: ");
    Serial.println(windSpeed);

    //Representación del clima
    if(weatherCode/100 == 2){   //Tormentas
        if(weatherCode == 211 || weatherCode == 212 || weatherCode == 221){ //Solo tormenta
            pintarTIEMPO(5, leds, 0, 0);
        }
        else    //Tormenta con lluvia
        {
            pintarTIEMPO(6, leds, 0, 0);
        }   
    }
    else if(weatherCode/100 == 3 || weatherCode == 500) //Llovizna
    {
        pintarTIEMPO(3, leds, 0, 0);
    }
    else if(weatherCode/100 == 5)   //Lluvia
    {
        pintarTIEMPO(4, leds, 0, 0);
    }
    else if(weatherCode/100 == 6)   //Granizo/nieve
    {
        pintarTIEMPO(7, leds, 0, 0);
    }
    else if(weatherCode/100 == 7)   //Niebla o condiciones adversas
    {
        pintarTIEMPO(8, leds, 0, 0);
    }
    else if(weatherCode/100 == 8)   //Claro/nubes
    {
        if(weatherCode == 800)  //Despejado
        {
            pintarTIEMPO(0, leds, 0, 0);
        }
        else if(weatherCode == 801 || weatherCode == 802 || weatherCode == 803)   //Claro con nubes
        {
            pintarTIEMPO(1, leds, 0, 0);
        }
        else if(weatherCode == 804)   //Nublado
        {
            pintarTIEMPO(2, leds, 0, 0);
        }
    }

    //Representación del viento
    if(windSpeed >= 21.0 && windSpeed < 40.0)
    {
        pintarVIENTO(0, leds, 13, -1);
    }
    else if(windSpeed >= 40.0)
    {
        pintarVIENTO(1, leds, 13, -1);
    }
}

//Aumenta o disminuye gradualmente el brillo de la pantalla, si apagadoEncendido es 0, tiende a apagarse y si es 1 se enciende.
void efectoFundido(int paso, int velocidad, int apagadoEncendido)
{
    if (apagadoEncendido == 0)
    {
        while (FastLED.getBrightness() > 10)
        {
            FastLED.setBrightness(FastLED.getBrightness() - paso);
            delay(velocidad);
            FastLED.show();
        }
    }
    else
    {
        while (FastLED.getBrightness() < BRILLO)
        {
            FastLED.setBrightness(FastLED.getBrightness() + paso);
            delay(velocidad);
            FastLED.show();
        }
    }
}

//Devuelve uno cuando la diferencia en tiempo entre los dos primeros parámetros es mayor o igual que el tiempo a esperar
bool esTiempo(unsigned long tiempoAnterior, unsigned long tiempoActual, unsigned int tiempoAEsperar)
{
    return (tiempoActual - tiempoAnterior >= tiempoAEsperar) ?  true:  false;
}


void administradorDeEscenas(int* escenario)
{   
    if(digitalRead(BOTONACCION) == 0){
        Serial.print("===PULSADO BOTON= ");
        Serial.print(*escenario);
        Serial.print("////");
        *escenario < 1 ? *escenario = *escenario + 1 : *escenario = 0;
        Serial.println(*escenario);
        delay(600); //Para evitar rebotes del botón
    }
}



