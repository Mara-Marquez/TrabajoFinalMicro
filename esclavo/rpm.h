#ifndef RPM_H
#define RPM_H

#include <Arduino.h>
#include <Arduino.h>

class Tacometro
{
public:
    void iniciar();
    void actualizar();
    float obtenerRPM();

private:
    int rpm = 0;
};

extern volatile uint32_t pulsos;

#endif