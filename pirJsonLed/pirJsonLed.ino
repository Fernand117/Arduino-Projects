#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"

#define IP "192.168.1.69" // DIRECCION IP DEL SERVIDOR
#define PORT 80           // PUERTO DEL SERVIDOR

RestClient client = RestClient(IP, PORT); //SE CREA UN NUEVO CLIENTE REST

byte pir = 2; //PIN DIGITAL DEL SENSOR DE MOVIMIENTO
int ledA = 4;
int ledV = 7;
int ledR = 8;
int val = 0;

String texto_fila;
String datos = "Mov";       //AQUÍ SE ALMACENARÁ LOS DATOS GENERADOS POR EL SENSOR
String nombre = "SHA";      //NOMBRE QUE SE LE DARÁ A CADA SENSOR
String tipo = "Movimiento"; //DEFINIR MOV O PROX PARA REDUCIR LAS CONSULTAS
String response;            //AQUÍ SE ALMACENA LA RESPUESTA DEL SERVIDOR

void setup()
{
  pinMode(pir, INPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledR, OUTPUT);
  
  Serial.begin(9600);
  
  digitalWrite(ledA, HIGH);
  digitalWrite(ledV, HIGH);
  digitalWrite(ledR, HIGH);

  digitalWrite(ledA, LOW);
  
  bool mensaje = client.dhcp();
  Serial.println(mensaje);
  if (mensaje == 0)
  {
      digitalWrite(ledA, HIGH);
  }
}

void loop()
{
  digitalWrite(ledV, LOW);
  val = digitalRead(pir);

  if (val == 1)
  {
    digitalWrite(ledV, HIGH);
    datos = "Movimiento detectado";
  }
  if (val == HIGH)
  {
    digitalWrite(ledR, LOW);
    Serial.println("Movimiento detectado");

    response = "";

    client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
    client.setHeader("Content-Type: application/json");

    StaticJsonBuffer<200> jsonBuffer;

    char json[256];

    JsonObject &root = jsonBuffer.createObject();

    root["nombre"] = nombre;
    root["datos"] = datos;
    root["tipoSensor"] = tipo;
    root.printTo(json, sizeof(json));

    Serial.println(json);

    int statusCode = client.post("/centinelaApi/api/registrar/datos-sensores", json, &response);

    texto_fila = "Status code from server: ";
    Serial.println(texto_fila);

    texto_fila = statusCode;
    Serial.println(texto_fila);

    texto_fila = "Response body from server: ";
    Serial.println(texto_fila);

    texto_fila = response;
    Serial.println(texto_fila);
    digitalWrite(ledR, HIGH);
  }
  else
  {
    Serial.println("No hay movimiento");
  }
  delay(3000);
}