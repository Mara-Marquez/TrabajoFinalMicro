#include "ventilador.h"

#define MAXTEMP 31.0

Ventilador::Ventilador(int pin)
    : pin(pin), controlPWM(pin)
{
}

void Ventilador::iniciar()
{
    controlPWM.iniciar();
}

int Ventilador::controlar(float temperatura, float setpoint)
{
    int pwm;

    if (temperatura < setpoint)
    {
        pwm = 0;
    }
    else
    {
        pwm = map(
            (int)(temperatura * 10),
            (int)(setpoint * 10),
            (int)(MAXTEMP * 10),
            80,
            255);

        pwm = constrain(pwm, 80, 255);
    }

    controlPWM.setDuty(pwm);

    return pwm;
}