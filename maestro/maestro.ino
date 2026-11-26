#include <Wire.h>
#include "pantalla.h"

#define SLAVE_ADDR 8

struct DatosSistema
{
    float temperatura;
    float setpoint;
    int rpm;
    int pwm;
    bool onof;
};

DatosSistema datos;
String command;
Pantalla pantalla;

void solicitarDatos()
{
    int recibidos =
        Wire.requestFrom(
            SLAVE_ADDR,
            sizeof(DatosSistema)
        );

   // Serial.print("Bytes recibidos: ");
   // Serial.println(recibidos);

    if (recibidos == sizeof(DatosSistema))
    {
        Wire.readBytes(
            (char*)&datos,
            sizeof(DatosSistema)
        );
    }
}

void enviarSetpoint(float nuevoSP)
{
    Wire.beginTransmission(SLAVE_ADDR);

    Wire.write(
        (byte*)&nuevoSP,
        sizeof(float)
    );

    Wire.endTransmission();
}

void mostrarSerial()
{
    String estado="";
     Serial.println("-------------------------");
    Serial.print("Temp: ");
    Serial.print(datos.temperatura);

    Serial.print(" SP: ");
    Serial.print(datos.setpoint);

    Serial.print(" RPM: ");
    Serial.print(datos.rpm);

    Serial.print(" PWM: ");
    Serial.print(datos.pwm);
if(datos.onof){
    estado="ON";

}else{ estado="OFF"; }
     

     Serial.print(" Estado: ");

    Serial.println(estado);
    Serial.println("-------------------------");
}

void setup()
{
    Serial.begin(9600);

    Wire.begin();

    if (!pantalla.iniciar())
    {
        Serial.println("Error OLED");
        while (1);
    }

    pantalla.mostrarMensaje(
        "Sistema",
        "Iniciando"
    );

    delay(200);
}

void loop()
{
    // Recibe datos actuales del esclavo
    solicitarDatos();

    // Los muestra
   // mostrarSerial();

    pantalla.mostrarDatos(
        datos.temperatura,
        datos.setpoint,
        datos.rpm,
        datos.pwm,
        datos.onof
    );

   

    // Lo envía al esclavo
if (Serial.available())
	{
    	command = Serial.readStringUntil('\n');
    	command.trim();
    	if (command.equalsIgnoreCase("A"))
    	{
        	solicitarDatos();
        	mostrarSerial();
    	}
	}
	


    

   
}