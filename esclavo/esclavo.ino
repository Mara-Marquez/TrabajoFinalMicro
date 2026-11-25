//Librerias
#include "dh11.h"
#include "potenciometro.h"
#include "ventilador.h"
#include <Wire.h>

//variables
#define SLAVE_ADDR 8
Temperatura sensorTemp(2);
Potenciometro pot(A3);
Pantalla pantalla;
Ventilador ventilador(9);
struct DatosSistema
{
    float temperatura;
    float setpoint;
    int rpm;
    int pwm;
};
void enviarDatos()
{
    Wire.write(
        (byte*)&datos,
        sizeof(datos)
    );
}

void setup()
{
    Wire.begin(SLAVE_ADDR);

Wire.onRequest(enviarDatos);
 Serial.begin(9600);
 sensorTemp.iniciar();
  pot.iniciar();
 
   ventilador.iniciar();

  
    
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
datos.rpm = rpm;
datos.pwm = porcentajePWM;


}
