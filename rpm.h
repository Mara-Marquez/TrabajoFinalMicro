#ifndef RPM_H
#define RPM_H

#include <Arduino.h>

class SensorRPM
{
private:
    int pinLedIR;
    int pinSensor;

    static volatile unsigned long contadorFlancos;

    unsigned long tiempoAnterior;
    unsigned long intervaloVentana;
    int rpmActual;

    static void contarPulso();

public:
    SensorRPM(int pinLed, int pinSensor);

    void iniciar();
    int calcular();
};

#endif