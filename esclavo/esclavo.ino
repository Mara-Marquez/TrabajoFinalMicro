//Librerias
#include "dh11.h"
#include "potenciometro.h"
#include "ventilador.h"
#include <Wire.h>

//variables
#define SLAVE_ADDR 8
Temperatura sensorTemp(2);
Potenciometro pot(A3);

Ventilador ventilador(9);
struct DatosSistema
{
    float temperatura;
    float setpoint;
    int rpm;
    int pwm;
};
DatosSistema datos;



void setup()
{
    Wire.begin(SLAVE_ADDR);

Wire.onReceive(receiveEvent);
Wire.onRequest(requestEvent);


 Serial.begin(9600);
 sensorTemp.iniciar();
pot.iniciar();
 
ventilador.iniciar();

  Serial.println("Esclavo iniciado");
    
}
void loop()
{
 float temperatura =
        sensorTemp.leer();
float setpoint =
        pot.obtenerSetpoint();

 int pwm = ventilador.controlar(temperatura, setpoint);


Serial.print("temperatura: ");
Serial.println(temperatura);

Serial.print("Setpoint: ");
Serial.println(setpoint);
int porcentajePWM =
    (pwm * 100) / 255;


datos.temperatura = temperatura;
datos.setpoint = setpoint;
datos.rpm = 0;
datos.pwm = porcentajePWM;


}


void requestEvent()
{
    Wire.write(
        (byte*)&datos,
        sizeof(DatosSistema)
    );

    Serial.print("Enviado -> ");
    Serial.print(datos.temperatura);
    Serial.print(" ");
    Serial.print(datos.setpoint);
    Serial.print(" ");
    Serial.print(datos.rpm);
    Serial.print(" ");
    Serial.println(datos.pwm);
}

void receiveEvent(int bytes)
{
    if(bytes == sizeof(float))
    {
        float nuevoSP;

        Wire.readBytes(
            (char*)&nuevoSP,
            sizeof(float)
        );

        datos.setpoint = nuevoSP;

        Serial.print("Nuevo Setpoint: ");
        Serial.println(datos.setpoint);
    }
}