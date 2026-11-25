#ifndef VENTILADOR_H
#define VENTILADOR_H

#include <Arduino.h>
#include "PWM.h"

class Ventilador
{
private:
    int pin;
    PWM controlPWM;

public:
    Ventilador(int pin);
    void iniciar();
    int controlar(float temperatura, float setpoint);
};

#endif