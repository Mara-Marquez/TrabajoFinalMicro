#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

class PWM
{
private:
    int pinPWM;

public:
    PWM(int pin);
    void iniciar();
    void setDuty(int duty);
};

#endif