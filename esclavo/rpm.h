#ifndef RPM_H
#define RPM_H

#include <Arduino.h>

class SensorRPM
{
private:
    int pinLedIR;
    int pinSensor;
    int umbralLuz;

    unsigned long tiempoAnterior;
    unsigned long intervaloVentana;

    int contadorFlancos;
    bool estadoAnteriorAlto;
    int rpmActual;

public:
    SensorRPM(int pinLed, int pinSensor);

    void iniciar();
    int calcular();
};

#endif
