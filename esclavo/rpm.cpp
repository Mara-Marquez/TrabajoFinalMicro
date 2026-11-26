#include "rpm.h"

SensorRPM::SensorRPM(int pinLed, int pinSensor)
{
    this->pinLedIR = pinLed;
    this->pinSensor = pinSensor;

    umbralLuz = 500;

    tiempoAnterior = 0;
    intervaloVentana = 3000;

    contadorFlancos = 0;
    estadoAnteriorAlto = false;
    rpmActual = 0;
}

void SensorRPM::iniciar()
{
    pinMode(pinLedIR, OUTPUT);

    int valorInicial = analogRead(pinSensor);

    estadoAnteriorAlto =
        (valorInicial > umbralLuz);

    tiempoAnterior = millis();
}

int SensorRPM::calcular()
{
    digitalWrite(pinLedIR, HIGH);

    int valorActual =
        analogRead(pinSensor);

    bool estadoActualAlto =
        (valorActual > umbralLuz);

    if (estadoActualAlto != estadoAnteriorAlto)
    {
        contadorFlancos++;
        estadoAnteriorAlto =
            estadoActualAlto;
    }

    unsigned long tiempoActual =
        millis();

    if (tiempoActual - tiempoAnterior >= intervaloVentana)
    {
        float revoluciones =
            (float)contadorFlancos / 14.0;

        rpmActual =
            (int)(revoluciones * 20.0);

        contadorFlancos = 0;
        tiempoAnterior = tiempoActual;
    }

    return rpmActual;
}