#ifndef RPM_H
#define RPM_H

#include <Arduino.h>

class SensorRPM {
  private:
    int pinSensor;
    int numeroAspas;
    unsigned long tiempoAnterior;
    int rpmActual;

  public:
    SensorRPM(int pin, int aspas);
    void iniciar();
    int calcular();
};

#endif