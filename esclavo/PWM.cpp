#include "PWM.h"

PWM::PWM(int pin)
{
    pinPWM = pin;
}

void PWM::iniciar()
{
    pinMode(pinPWM, OUTPUT);
}

void PWM::setDuty(int duty)
{
    duty = constrain(duty, 0, 255);
    analogWrite(pinPWM, duty);
}