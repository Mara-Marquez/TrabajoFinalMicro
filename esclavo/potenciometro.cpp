#include "Potenciometro.h"
#include <Arduino.h>

#define MINTEMP 22.0
#define MAXTEMP 30.0

Potenciometro::Potenciometro(int pin)
{
    this->pin = pin;
}

void Potenciometro::iniciar()
{
    pinMode(pin, INPUT);
}

int Potenciometro::obtenerValor()
{
    return analogRead(pin);
}

int Potenciometro::obtenerPWM()
{
    int valorPOT = analogRead(pin);
    return map(valorPOT, 0, 1023, 0, 255);
}

int Potenciometro::obtenerSetpoint()
{
    int valor = analogRead(pin);

      return MINTEMP +
           (valor * (MAXTEMP - MINTEMP) / 1023.0);
}