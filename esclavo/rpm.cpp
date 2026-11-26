#include "rpm.h"

volatile unsigned long contadorPulsos = 0;
volatile unsigned long ultimoPulsoMicros = 0;

void contarPulsoISR() {
  unsigned long ahora = micros();
  if (ahora - ultimoPulsoMicros > 5000) {
  contadorPulsos++;
  ultimoPulsoMicros = ahora;
  }
}

SensorRPM::SensorRPM(int pin, int aspas) {
  pinSensor = pin;
  numeroAspas = aspas;
  tiempoAnterior = 0;
  rpmActual = 0;
}

void SensorRPM::iniciar() {
  pinMode(pinSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSensor), contarPulsoISR, FALLING);
  tiempoAnterior = millis();
}

int SensorRPM::calcular() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= 1000) {
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    // RPM = (pulsos por segundo * 60 segundos) / cantidad de aspas que cortan el haz IR
    rpmActual = (int)((pulsos * 60UL) / (unsigned long)numeroAspas);
    tiempoAnterior = tiempoActual;
  }

  return rpmActual;
}