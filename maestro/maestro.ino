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

void enviarOnOff(bool estado)
{
    Wire.beginTransmission(SLAVE_ADDR);

    Wire.write(
        (byte*)&estado,
        sizeof(bool)
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
    Serial.print(datos.pwm);

    Serial.print(" ONOF: ");
    Serial.println(datos.onof ? "ON" : "OFF");
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

    if (Serial.available() > 0)
    {
        char tecla = Serial.read();

        if (tecla == 'A' || tecla == 'a')
        {
            solicitarDatos();
            mostrarSerial();
        }
        else if (tecla == 'E' || tecla == 'e')
        {
            enviarOnOff(true);
            Serial.println("ON");
        }
        else if (tecla == 'O' || tecla == 'o')
        {
            enviarOnOff(false);
            Serial.println("OFF");
        }
    }
    // Recibe datos actuales del esclavo
    solicitarDatos();

    pantalla.mostrarDatos(
        datos.temperatura,
        datos.setpoint,
        datos.rpm,
        datos.pwm,
        datos.onof
    );

    delay(150);
}