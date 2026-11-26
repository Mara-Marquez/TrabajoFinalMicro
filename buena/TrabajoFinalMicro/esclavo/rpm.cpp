#include "rpm.h"

SensorRPM::SensorRPM(int pinLed, int pinAnalogico) {
  pinLedIR = pinLed;
  pinSensor = pinAnalogico;
  umbralLuz = 450;             // Umbral para separar luz de sombra
  intervaloVentana = 3000;     // Ventana de medición de 3 segundos
  tiempoAnterior = 0;
  contadorFlancos = 0;
  estadoAnteriorAlto = false;
  rpmActual = 0;
}

void SensorRPM::iniciar() {
  pinMode(pinLedIR, OUTPUT);
  digitalWrite(pinLedIR, HIGH); // Enciende el LED infrarrojo permanentemente

  // Sincronización inicial del estado del sensor
  int valorInicial = analogRead(pinSensor);
  estadoAnteriorAlto = (valorInicial > umbralLuz);
  tiempoAnterior = millis();
}

int SensorRPM::calcular() {
  digitalWrite(pinLedIR, HIGH);

  int valorActual = analogRead(pinSensor);
  bool estadoActualAlto = (valorActual > umbralLuz);

  // Detecta cada cambio de estado (interrupción o liberación del haz)
  if (estadoActualAlto != estadoAnteriorAlto) {
    contadorFlancos++;
    estadoAnteriorAlto = estadoActualAlto;
  }

  // Cada 3 segundos actualiza el cálculo de RPM sin bloquear el loop
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= intervaloVentana) {
    // 7 aspas = 14 flancos por vuelta; proyección de 3s a 60s (* 20)
    float revoluciones = (float)contadorFlancos / 14.0;
    rpmActual = (int)(revoluciones * 20.0);

    contadorFlancos = 0;
    tiempoAnterior = tiempoActual;
  }

  return rpmActual;
}