//Librerias
#include "dh11.h"
#include "potenciometro.h"
#include "ventilador.h"
#include <Wire.h>
#include "Watchdog.h"
#include <avr/interrupt.h>

//variables
#define SLAVE_ADDR 8
Temperatura sensorTemp(2);
Potenciometro pot(A3);
Watchdog watchdog;
Ventilador ventilador(9);
struct DatosSistema
{
    float temperatura;
    float setpoint;
    int rpm;
    int pwm;
    bool ONOF;
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

  
   watchdog.iniciar(); 
   Serial.println("Esclavo iniciado");
}
void loop()
{
    watchdog.reset();
 float temperatura =
        sensorTemp.leer();

 if (isnan(temperatura))
{
  Serial.println("Reiniciando");

    delay(1000);

    watchdog.reiniciarSistema();
}

float setpoint =
        pot.obtenerSetpoint();

 int pwm = ventilador.controlar(temperatura, setpoint);



int porcentajePWM =
    (pwm * 100) / 255;
    bool onof=false;
if(porcentajePWM>0){
onof=true;
}
else{
    onof=false;
}
cli();

datos.temperatura = temperatura;
datos.setpoint = setpoint;
datos.rpm = 0;
datos.pwm = porcentajePWM;
datos.ONOF =onof;
sei();

}


void requestEvent()
{
    Wire.write(
        (byte*)&datos,
        sizeof(DatosSistema)
    );
/*
    Serial.print("Enviado -> ");
    Serial.print(datos.temperatura);
    Serial.print(" ");
    Serial.print(datos.setpoint);
    Serial.print(" ");
    Serial.print(datos.rpm);
    Serial.print(" ");
    Serial.println(datos.pwm);
    */
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

       // Serial.print("Nuevo Setpoint: ");
      //  Serial.println(datos.setpoint);
    }
}