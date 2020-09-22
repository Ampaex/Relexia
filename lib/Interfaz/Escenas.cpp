//          CÓDIGO FUENTE DE CONTROL DE ESCENAS RELEXIA
//                Autor: Antonio Vázquez Pérez

#include "Escenas.h"

//----------------------ESCENAS DISPONIBLES---------------------------

//Escenario 0
void escenaReloj(RtcDS1302<ThreeWire> rtc, CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo)
{
    RtcDateTime fechaAnterior = obtenerFechaRTC(rtc);
    RtcDateTime fechaActual;
    DynamicJsonDocument meteo(5000);//15000
    FastLED.setBrightness(0);
    meteo = obtenerMeteo(0);


    //Pinta la fecha que acabamos de leer
    borrarPixeles(leds);
    pintarNUM(fechaAnterior.Hour() / 10, CRGB::White, leds, 4, -2);
    pintarNUM(fechaAnterior.Hour() % 10, CRGB::White, leds, 9, -2);
    pintarNUM(fechaAnterior.Minute() / 10, CRGB::White, leds, 4, -9);
    pintarNUM(fechaAnterior.Minute() % 10, CRGB::White, leds, 9, -9);

    mostrarMeteo(leds, meteo);
    FastLED.show();
    efectoFundido(1, 400 / *brillo, *brillo);
    
    unsigned long millisFecha = millis();

    while(*escenario == 0)
    {
        //Comprueba cada 3s
        if(esTiempo(millisFecha, millis(), 3000)){
            millisFecha = millis();
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

            compruebaDormir(*ultimaAct, leds);

            if (estadoWifi() == 0)
            {
                conectaWiFi();
            }

            conexionAlexa->handle();

        }
        
        administradorDeEscenas(escenario, ultimaAct);
    }
    
}

//Escenario 1
void escenaPrevision(CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo)
{
    DynamicJsonDocument meteo(15000);
    FastLED.setBrightness(0);
    meteo = obtenerMeteo(1);
    borrarPixeles(leds);

    //Representamos la meteorología primero
    mostrarMeteo(leds, meteo);

    float maximas = meteo["daily"][1]["temp"]["max"];
    float minimas = meteo["daily"][1]["temp"]["min"];
    Serial.printf("Temperaturas máximas: %f \n", maximas);
    Serial.printf("Temperaturas mínimas: %f \n", minimas);

    //Representamos temperaturas mínimas
    pintarNUM((int)maximas / 10, 0xBD0707, leds, 1, -2);
    pintarNUM((int)maximas % 10, 0xBD0707, leds, 5, -2);
    leds[desplazaPixel(0, 9, -6)] = 0xBD0707; //Punto
    pintarNUM((int)(maximas*10) % 10, 0xBD0707, leds, 11, -2);
    leds[desplazaPixel(0, 15, -2)] = 0xBD0707; //Cerito

    //Representamos temperaturas máximas
    pintarNUM((int)minimas / 10, 0x00A2E2, leds, 1, -9);
    pintarNUM((int)minimas % 10, 0x00A2E2, leds, 5, -9);
    leds[desplazaPixel(0, 9, -13)] = 0x00A2E2; //Punto
    pintarNUM((int)(minimas*10) % 10, 0x00A2E2, leds, 11, -9);
    leds[desplazaPixel(0, 15, -9)] = 0x00A2E2; //Cerito

    efectoFundido(1, 400 / *brillo, *brillo);

    unsigned long millisTemp = millis();
    while(*escenario == 1){

        administradorDeEscenas(escenario, ultimaAct);

        if (esTiempo(millisTemp, millis(), 3000))
        {
            millisTemp = millis();
            
            compruebaDormir(*ultimaAct, leds);

            //Se comprueban las conexiones con alexa
            if (estadoWifi() == 0)
            {
                conectaWiFi();
            }
            conexionAlexa->handle();
        }
    }

}

//Escenario 2
void escenaTempHum(AHT10 myAHT10, CRGB* leds, int* escenario, unsigned long* ultimaAct, fauxmoESP* conexionAlexa, int* brillo)
{

    float temperatura = obtenerTemperatura(myAHT10);
    float humedad = obtenerHumedad(myAHT10);
    FastLED.setBrightness(0);
    
    borrarPixeles(leds);
    //Representar temperatura
    pintarNUM((int)temperatura / 10, 0xFF6F00, leds, 1, -2);
    pintarNUM((int)temperatura % 10, 0xFF6F00, leds, 5, -2);
    leds[desplazaPixel(0, 9, -6)] = 0xFF6F00; //Punto
    pintarNUM((int)(temperatura*10) % 10, 0xFF6F00, leds, 11, -2);
    leds[desplazaPixel(0, 15, -2)] = 0xFF6F00; //Ceritotemperatura

    //Representar humedad
    pintarNUM((int)humedad / 10, 0x00A2E2, leds, 3, -9);
    pintarNUM((int)humedad % 10, 0x00A2E2, leds, 7, -9);
    pintarICONO(0, 0x00A2E2, leds, 11, -9);

    efectoFundido(1, 400 / *brillo, *brillo);
    
    unsigned long millisTemp = millis();
    while(*escenario == 2){
        administradorDeEscenas(escenario, ultimaAct);

        if (esTiempo(millisTemp, millis(), 3000))
        {
            millisTemp = millis();
            
            compruebaDormir(*ultimaAct, leds);

            //Se comprueban las conexiones con alexa
            if (estadoWifi() == 0)
            {
                conectaWiFi();
            }
            conexionAlexa->handle();
        }
    }
}




//Escenario 3
void escenaLuz(CRGB *leds, int *escenario, fauxmoESP *conexionAlexa, int *brillo, int* color)
{

    borrarPixeles(leds);
    FastLED.setBrightness(0);

    for (int i = 0; i < NUMPIXELS; i++)
    {
        leds[i] = *color + random(0, 80); //Color azul
    }

    efectoFundido(1, 400 / *brillo, *brillo);

    unsigned long contador1 = millis();
    unsigned long contador2 = contador1;
    while (*escenario == 3)
    {
        if (esTiempo(contador1, millis(), 600))
        {
            contador1 = millis();
            for (int i = 0; i < (NUMPIXELS - 1); i++)
            {
                leds[i] = leds[i + 1];
            }
            leds[NUMPIXELS - 1] = *color + random(0, 80);
            FastLED.show();
        }

        if (esTiempo(contador2, millis(), 2000))
        {
            contador2 = millis();
            conexionAlexa->handle();
        }
    }
}




//Dada una meteorología la muestra en pantalla clasificada correctamente
void mostrarMeteo(CRGB* leds, DynamicJsonDocument meteo)
{
    
    int weatherCode;
    float windSpeed;
    //Comprobamos si es un json de previsión de estado actual del tiempo
    String prueba = meteo["daily"][1]["weather"][0]["id"];
    if (prueba == "null")
    {
        weatherCode = meteo["weather"][0]["id"];
        windSpeed = meteo["wind"]["speed"];
    }
    else
    {
        weatherCode = meteo["daily"][1]["weather"][0]["id"];
        windSpeed = meteo["daily"][1]["wind_speed"];
    }

    Serial.print("Tiempo: ");
    Serial.print(weatherCode);
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
void efectoFundido(int velocidad, int apagadoEncendido, int brillo)
{
    if (apagadoEncendido == 0)
    {
        while (FastLED.getBrightness() > 10)
        {
            FastLED.setBrightness(FastLED.getBrightness() - 1);
            delay(velocidad);
            FastLED.show();
        }
    }
    else
    {
        while (FastLED.getBrightness() < brillo)
        {
            FastLED.setBrightness(FastLED.getBrightness() + 1);
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


void administradorDeEscenas(int* escenario, unsigned long* ultimaActividad)
{   
    if(digitalRead(BOTONACCION) == 0){
        *ultimaActividad = millis();
        Serial.print(*escenario);
        Serial.print("////");
        *escenario < 2 ? *escenario = *escenario + 1 : *escenario = 0;
        Serial.println(*escenario);
        delay(600); //Para evitar rebotes del botón
    }
    else if(digitalRead(WAKE_PIN) == 1 && esTiempo(*ultimaActividad, millis(), 1000))       //Comprueba si se está detectando movimiento
    {
        *ultimaActividad = millis();
    }
}




