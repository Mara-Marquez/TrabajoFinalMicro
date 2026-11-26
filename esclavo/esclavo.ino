//Librerias
#include "dh11.h"
#include "potenciometro.h"
#include "ventilador.h"
#include "rpm.h"
#include <Wire.h>
#include "Watchdog.h"

//variables
#define SLAVE_ADDR 8
Temperatura sensorTemp(2);
Potenciometro pot(A3);
Watchdog watchdog;
Ventilador ventilador(9);
SensorRPM sensorRPM(4, A0);

struct DatosSistema
{
    float temperatura;
    float setpoint;
    int rpm;
    int pwm;
    bool ONOF;
};

DatosSistema datos;
bool modoManual= false;    

void setup()
{
    Wire.begin(SLAVE_ADDR);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    Serial.begin(9600);
    sensorTemp.iniciar();
    pot.iniciar();
    ventilador.iniciar();
    sensorRPM.iniciar();
  
    watchdog.iniciar(); 
    Serial.println("Esclavo iniciado");
}

void loop()
{
    watchdog.reset();
    float temperatura = sensorTemp.leer();

    if (isnan(temperatura))
    {
        Serial.println("Reiniciando");
        delay(1000);
        watchdog.reiniciarSistema();
    }

    float setpoint = pot.obtenerSetpoint();

    int pwm = 0;
    if (modoManual)
    {
        pwm = ventilador.controlar(50.0, 10.0); 
    }
    else
    {
        pwm = ventilador.controlar(temperatura, setpoint);
    }

    int porcentajePWM = (pwm * 100) / 255;
    bool onof = (porcentajePWM > 0);
     noInterrupts();
    datos.temperatura = temperatura;
    datos.setpoint = setpoint;
    datos.rpm = sensorRPM.calcular();
    datos.pwm = porcentajePWM;
    datos.ONOF = onof;
    interrupts();
}

void requestEvent()
{
    Wire.write(
        (uint8_t*)&datos,
        sizeof(datos)
    );
}

void receiveEvent(int bytes)
{
    if (bytes == sizeof(bool))
    {
        Wire.readBytes(
            (char*)&modoManual,
            sizeof(bool)
        );

        Serial.print("Modo Manual: ");
        Serial.println(modoManual ? "ACTIVADO (Forzado ON)" : "DESACTIVADO (Automatico)");
    }
    else
    {
        while (Wire.available())
        {
            Wire.read();
        }
    }
}