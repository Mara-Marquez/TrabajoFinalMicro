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

Pantalla pantalla;

void solicitarDatos()
{
    int recibidos =
        Wire.requestFrom(
            SLAVE_ADDR,
            sizeof(DatosSistema)
        );

    Serial.print("Bytes recibidos: ");
    Serial.println(recibidos);

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
    Serial.print("Temp: ");
    Serial.print(datos.temperatura);

    Serial.print(" SP: ");
    Serial.print(datos.setpoint);

    Serial.print(" RPM: ");
    Serial.print(datos.rpm);

    Serial.print(" PWM: ");
    Serial.println(datos.pwm);
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

    delay(2000);
}

void loop()
{
    // Recibe datos actuales del esclavo
    solicitarDatos();

    // Los muestra
    mostrarSerial();

    pantalla.mostrarDatos(
        datos.temperatura,
        datos.setpoint,
        datos.rpm,
        datos.pwm,
        datos.onof
    );

    // Calcula un nuevo setpoint
    float nuevoSP = datos.setpoint + 1.0;

    // Lo envía al esclavo
    enviarSetpoint(nuevoSP);

    Serial.print("Nuevo SP enviado: ");
    Serial.println(nuevoSP);

    delay(3000);
}