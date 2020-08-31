//          CÓDIGO FUENTE LIBRERÍA DE CONTROL DE PERIFÉRICOS RELEXIA
//                Autor: Antonio Vázquez Pérez
//    Fecha de creación: 20/03/2020
//       Última versión: 7/06/2020

#include "Perifericos.h"
#include "Red.h"

//----------------------------------------------SD--------------------------------------------------

int inicializaSD()
{
    if (!SD.begin())
    {
        Serial.println("No se pudo montar la tarjeta");
        return 1;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("Ninguna SD conectada");
        return 1;
    }

    Serial.println("SD montada correctamente");
    return 0;
}

//Funciones de ejemplo de uso de SD
//https://github.com/espressif/arduino-esp32/blob/master/libraries/SD/examples/SD_Test/SD_Test.ino
void leeArchivo(fs::FS &fs, const char *path)
{
    Serial.printf("Leyendo archivo: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Fallo abriendo archivo para leer");
        return;
    }

    Serial.print("Lectura de archivo: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

//-------------------------------------------PANTALLA----------------------------------------------

void inicializaPantalla(CRGB* leds)
{
    FastLED.addLeds<WS2812B, PANTPIN, GRB>(leds, NUMPIXELS);
    FastLED.setBrightness(BRILLO);
}

int desplazaPixel(int posicion, int xOffs, int yOffs)
{
    int tempPos = posicion;
    int contX = xOffs;

    //Repetimos la operación de desplazar horizontalmente tantas veces como pide xOffs

    while (contX != 0)
    {
        //Comprobamos si se encuentra en una columna par o impar para el desplazamiento horizontal
        if ((tempPos / 16) % 2 == 0) //Par
        {
            if (xOffs > 0) //Desplaza derecha -> 31 - 2 * (dist. arriba)
            {
                tempPos += 31 - 2 * (tempPos % 16);
            }
            else //Desplaza izquierda
            {
                tempPos -= 1 + 2 * (tempPos % 16);
            }
        }
        else //Impar
        {
            if (xOffs > 0) //Desplaza derecha (como este caso nos da la dist. abajo hay que invertirla con 15 - ...)
            {
                tempPos += 1 + 2 * (15 - (tempPos % 16));
            }
            else //Desplaza izquierda
            {
                tempPos -= 31 - 2 * (15 - (tempPos % 16));
            }
        }

        contX > 0 ? contX-- : contX++;
    }

    //Comprobamos si se encuentra en una columna par o impar para el desplazamiento vertical
    if ((tempPos / 16) % 2 == 0) //Par
    {
        tempPos -= yOffs;
    }
    else    //Impar
    {
        tempPos += yOffs;
    }
    
    return tempPos;
}

void borrarPixeles(CRGB* leds)
{
    for (int i = 0; i< NUMPIXELS; i++)
    {
        leds[i] = CRGB::Black;
    }
}

/*Coloca en ram de la pantalla un número(se debe hacer show después)
    Números 1-9 -> Su correspondiente número
    Número 10 -> Caracter blanco(Para borrar)
    Número desconocido -> !
*/
void pintarNUM(int num, CRGB color, CRGB *leds, int xOffs, int yOffs)
{
    switch (num)
    {
    case 0:
    {
        int cero[12] = {0, 1, 2, 3, 4, 27, 31, 32, 33, 34, 35, 36};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(cero[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 1:
    {
        int uno[8] = {0, 4, 27, 28, 29, 30, 31, 36};
        for (int i = 0; i < 8; i++)
        {
            leds[desplazaPixel(uno[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 2:
    {
        int dos[11] = {0, 2, 3, 4, 27, 29, 31, 32, 33, 34, 36};
        for (int i = 0; i < 11; i++)
        {
            leds[desplazaPixel(dos[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 3:
    {
        int tres[11] = {0, 2, 4, 27, 29, 31, 32, 33, 34, 35, 36};
        for (int i = 0; i < 11; i++)
        {
            leds[desplazaPixel(tres[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 4:
    {
        int cuatro[9] = {0, 1, 2, 29, 32, 33, 34, 35, 36};
        for (int i = 0; i < 9; i++)
        {
            leds[desplazaPixel(cuatro[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 5:
    {
        int cinco[11] = {0, 1, 2, 4, 27, 29, 31, 32, 34, 35, 36};
        for (int i = 0; i < 11; i++)
        {
            leds[desplazaPixel(cinco[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 6:
    {
        int seis[12] = {0, 1, 2, 3, 4, 27, 29, 31, 32, 34, 35, 36};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(seis[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 7:
    {
        int siete[7] = {0, 3, 4, 29, 31, 32, 33};
        for (int i = 0; i < 7; i++)
        {
            leds[desplazaPixel(siete[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 8:
    {
        int ocho[13] = {0, 1, 2, 3, 4, 27, 29, 31, 32, 33, 34, 35, 36};
        for (int i = 0; i < 13; i++)
        {
            leds[desplazaPixel(ocho[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 9:
    {
        int nueve[10] = {0, 1, 2, 29, 31, 32, 33, 34, 35, 36};
        for (int i = 0; i < 10; i++)
        {
            leds[desplazaPixel(nueve[i], xOffs, yOffs)] = color;
        }
        break;
    }

    case 10:
    {
        int blanco[15] = {0, 1, 2, 3, 4, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
        for (int i = 0; i < 15; i++)
        {
            leds[desplazaPixel(blanco[i], xOffs, yOffs)] = color;
        }
        break;
    }

    default: //Si no se corresponde con ningún número dibuja una exclamación
    {
        int excl[4] = {27, 29, 30, 31};
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(excl[i], xOffs, yOffs)] = color;
        }
        break;
    }
    }
}

/*Coloca en ram de la pantalla una figura de tiempo atmosférico según el selector
    0 -> Soleado
    1 -> Sol y nubes
    2 -> Nublado
    3 -> Lluvia leve
    4 -> Lluvia fuerte
    5 -> Tormenta
    6 -> Tormenta y lluvia
    7 -> Granizo / nieve
    8 -> Niebla o condiciones adversas
*/
void pintarTIEMPO(int selector, CRGB *leds, int xOffs, int yOffs)
{
    switch (selector)
    {
    case 0:
    {
        int solPos[10] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 63};
        for (int i = 0; i < 10; i++)
        {
            leds[desplazaPixel(solPos[i], xOffs, yOffs)] = 0xEFD600;
        }
        break;
    }

    case 1:
    {
        int solPos[10] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 63};
        int nubPos[7] = {2, 3, 30, 33, 62, 63, 64};
        for (int i = 0; i < 10; i++)
        {
            leds[desplazaPixel(solPos[i], xOffs, yOffs)] = 0xEFD600; //Amarillo
        }
        for (int i = 0; i < 7; i++)
        {
            leds[desplazaPixel(nubPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        break;
    }

    case 2:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        break;
    }

    case 3:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int gotPos[2] = {29, 63};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        for (int i = 0; i < 2; i++)
        {
            leds[desplazaPixel(gotPos[i], xOffs, yOffs)] = 0x30AAFF; //Azul
        }
        break;
    }

    case 4:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int gotPos[8] = {0, 3, 7, 29, 31, 37, 40, 63};
        
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        for (int i = 0; i < 8; i++)
        {
            leds[desplazaPixel(gotPos[i], xOffs, yOffs)] = 0x30AAFF; //Azul
        }
        break;
    }

    case 5:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int rayPos[4] = {28, 30, 34, 36};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(rayPos[i], xOffs, yOffs)] = 0xEFD600; //Amarillo
        }
        break;
    }

    case 6:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int rayPos[4] = {28, 30, 34, 36};
        int gotPos[4] = {2, 5, 39, 63};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x4B4B4B; //Gris
        }
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(rayPos[i], xOffs, yOffs)] = 0xEFD600; //Amarillo
        }
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(gotPos[i], xOffs, yOffs)] = 0x30AAFF; //Azul
        }

        break;
    }

    case 7:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int nievPos[4] = {1, 29, 31, 33};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x2D2D2D; //Gris oscuro
        }
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(nievPos[i], xOffs, yOffs)] = 0xFFFFFF; //Blanco
        }
        break;
    }

    case 8:
    {
        int nublPos[12] = {0, 1, 2, 3, 29, 30, 31, 32, 33, 62, 63, 64};
        int blanPos[6] = {0, 2, 29, 31, 32, 63};
        for (int i = 0; i < 12; i++)
        {
            leds[desplazaPixel(nublPos[i], xOffs, yOffs)] = 0x2D2D2D; //Gris oscuro
        }
        for (int i = 0; i < 6; i++)
        {
            leds[desplazaPixel(blanPos[i], xOffs, yOffs)] = 0xFFFFFF; //Blanco
        }
        break;
    }

        
    default:
        break;
    }
}


/*Coloca en ram de la pantalla una figura de banderín según el selector (tamaño NUM)
    0 -> Viento medio
    1 -> Viento fuerte
*/
void pintarVIENTO(int selector, CRGB *leds, int xOffs, int yOffs)
{
    switch (selector)
    {
    case 0:
    {
        int paloPos[5] = {0, 1, 2, 3, 4};
        int bandPos = 31;
        for (int i = 0; i < 5; i++)
        {
            leds[desplazaPixel(paloPos[i], xOffs, yOffs)] = 0x98705F;   //Marrón
        }
        leds[desplazaPixel(bandPos, xOffs, yOffs)] = 0xE30E0E;  //Rojo
        break;
    }

    case 1:
    {
        int paloPos[5] = {0, 1, 2, 3, 4};
        int bandPos[3] = {30, 31, 32};
        for (int i = 0; i < 5; i++)
        {
            leds[desplazaPixel(paloPos[i], xOffs, yOffs)] = 0x98705F;   //Marrón
        }
        leds[desplazaPixel(bandPos[0], xOffs, yOffs)] = 0xE30E0E;  //Rojo
        leds[desplazaPixel(bandPos[1], xOffs, yOffs)] = 0xE30E0E;
        leds[desplazaPixel(bandPos[2], xOffs, yOffs)] = 0xE30E0E;
        break;
    }
    
    default:
        break;
    }
}


/*Coloca en ram de la pantalla un icono(se debe hacer show después)
    Números 0 -> %
    Número desconocido -> !
*/
void pintarICONO(int num, CRGB color, CRGB *leds, int xOffs, int yOffs)
{

    switch (num)
    {
    case 0:
    {
        int porcen[7] = {0, 3, 4, 29, 32, 33, 36};
        for (int i = 0; i < 7; i++)
        {
            leds[desplazaPixel(porcen[i], xOffs, yOffs)] = color;
        }
        break;
    }

    default: //Si no se corresponde con ningún número dibuja una exclamación
    {
        int excl[4] = {27, 29, 30, 31};
        for (int i = 0; i < 4; i++)
        {
            leds[desplazaPixel(excl[i], xOffs, yOffs)] = color;
        }
        break;
    }
    }
}

//----------------------------------------------RTC--------------------------------------------------
void inicializaReloj(RtcDS1302<ThreeWire> Rtc)
{
    Rtc.Begin();

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
}

void cargaFecha(RtcDS1302<ThreeWire> Rtc)
{
    Serial.println("Estableciendo fecha en red...");
    StaticJsonDocument<5000> miJson = obtenerFecha(); //Consulta en red
    if(miJson.isNull())
    {
        Serial.println("Fallo en la petición http");
        return;
    }
    String dateTime = miJson["datetime"];

    //2020-06-07T22:42:26.452556+02:00
    RtcDateTime dt = RtcDateTime((uint16_t)dateTime.substring(0, 4).toInt(),
                                 (uint8_t)dateTime.substring(5, 7).toInt(),
                                 (uint8_t)dateTime.substring(8, 10).toInt(),
                                 (uint8_t)dateTime.substring(11, 13).toInt(),
                                 (uint8_t)dateTime.substring(14, 16).toInt(),
                                 (uint8_t)dateTime.substring(17, 19).toInt());

    imprimirFechaRTC(dt);

    Rtc.SetDateTime(dt);

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("Carga fallida, el reloj no contiene una fecha y hora válidas");
    }
    else
    {
        Serial.println("Fecha actual cargada");
    }
    
}

RtcDateTime obtenerFechaRTC(RtcDS1302<ThreeWire> Rtc)
{
    RtcDateTime res = Rtc.GetDateTime();
    return res;
}

void imprimirFechaRTC(RtcDateTime dt)
{
    Serial.print("Año:");
    Serial.println(dt.Year());
    Serial.print("Mes:");
    Serial.println(dt.Month());
    Serial.print("Día:");
    Serial.println(dt.Day());
    Serial.print("Hora:");
    Serial.println(dt.Hour());
    Serial.print("Minutos:");
    Serial.println(dt.Minute());
    Serial.print("Segundos:");
    Serial.println(dt.Second());
}

//Comprueba  si ha transcurrido el tiempo suficiente desde la última actividad para irse a dormir
void compruebaDormir(unsigned long millisUltimaAct, CRGB *leds)
{
    if (PUEDE_DORMIR && (millis() - millisUltimaAct >= TIEMPO_HASTA_SLEEP_MILLIS))
    {
        borrarPixeles(leds);
        FastLED.show();
        Serial.println("YÉNDOSE A DORMIR");
        esp_deep_sleep_start();
    }
}
//----------------------------------------HUMEDAD Y TEMPERATURA-------------------------------------------

void inicializaHumTemp(AHT10 myAHT10)
{

    if (myAHT10.begin() != true)
    {
    Serial.println(F("AHT10 no conectado o falló al carcar el coeficiente de calibración")); //Guarda en flash y mantiene la memoria dinámica libre
    delay(5000);
    }
    else Serial.println(F("AHT10 OK"));
}

float obtenerTemperatura(AHT10 myAHT10)
{
    return myAHT10.readTemperature();
}

float obtenerHumedad(AHT10 myAHT10)
{
    return myAHT10.readHumidity();
}

//------------------------------------------------BOTONES-----------------------------------------------

