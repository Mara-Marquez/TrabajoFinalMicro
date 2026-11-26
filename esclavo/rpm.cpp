#include "rpm.h"

const int pinSensor = 3;
const uint8_t PULSOS_POR_VUELTA = 7;

volatile uint32_t pulsos = 0;

void contarPulso()
{
    pulsos++;
}


void RPM::iniciar()
{
    pinMode(pinSensor, INPUT);

    attachInterrupt(
        digitalPinToInterrupt(pinSensor),
        contarPulso,
        FALLING
    );
      static unsigned long tiempoAnterior = 0;
}


void RPM::actualizar()
{
  

    if (millis() - tiempoAnterior >= 1000)
    {
        noInterrupts();
        uint32_t pulsosSegundo = pulsos;
        pulsos = 0;
        interrupts();

        rpm = (pulsosSegundo * 60.0) / PULSOS_POR_VUELTA;

        tiempoAnterior = millis();
    }
}

float RPM::obtenerRPM()
{
    return rpm;
}
