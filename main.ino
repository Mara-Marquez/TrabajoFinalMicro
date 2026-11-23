//Librerias
#include "dh11.h"
#include "potenciometro.h"
#include "pantalla.h"
#include "ventilador.h"


//variables
Temperatura sensorTemp(2);
Potenciometro pot(A3);
Pantalla pantalla;
Ventilador ventilador(9);


void setup()
{
 Serial.begin(9600);
 sensorTemp.iniciar();
  pot.iniciar();
  pantalla.iniciar();
   ventilador.iniciar();

    pantalla.mostrarMensaje(
        "Iniciando Sistema",
        "OK..."
    );
    
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

pantalla.mostrarDatos(
    temperatura,
    setpoint,
    porcentajePWM,
    0
    
);

}
