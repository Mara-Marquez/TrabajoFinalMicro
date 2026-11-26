#include "rpm.h"

volatile unsigned long SensorRPM::contadorFlancos = 0;

SensorRPM::SensorRPM(int pinLed, int pin)
{
    pinLedIR = pinLed;
    pinSensor = pin;

    tiempoAnterior = 0;
    intervaloVentana = 3000;
    rpmActual = 0;
}

void SensorRPM::contarPulso()
{
    contadorFlancos++;
}

void SensorRPM::iniciar()
{
    pinMode(pinLedIR, OUTPUT);
    digitalWrite(pinLedIR, HIGH);

    pinMode(pinSensor, INPUT);

    attachInterrupt(
        digitalPinToInterrupt(pinSensor),
        contarPulso,
        CHANGE
    );

    tiempoAnterior = millis();
}

int SensorRPM::calcular()
{
    unsigned long tiempoActual = millis();

    if (tiempoActual - tiempoAnterior >= intervaloVentana)
    {
        noInterrupts();
        unsigned long flancos = contadorFlancos;
        contadorFlancos = 0;
        interrupts();

        float revoluciones = flancos / 14.0;

        rpmActual = (int)(revoluciones * 20.0);

        tiempoAnterior = tiempoActual;
    }

    return rpmActual;
}