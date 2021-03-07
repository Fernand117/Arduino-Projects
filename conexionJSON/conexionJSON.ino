/*
 * LIBRERIAS UTILIZADAS PARA EL ENVIO DE DATOS
 * REST CLIENT FOR ARDUINO: https://github.com/csquared/arduino-restclient/
 * ARDUINO JSON: https://github.com/bblanchon/ArduinoJson
 * CLONAR O DESCARGAR EL CÓDIGO Y PASAR A LA CARPETA DE LIBRERÍAS
 * EL ARDUINO JSON HAY QUE BAJARLO A LA VERSIÓN 5.13.5
*/

#include <Ethernet.h>
#include <SPI.h>

#include <ArduinoJson.h>
#include "RestClient.h"

#define IP "192.168.1.69" // DIRECCION IP DEL SERVIDOR
#define PORT 80     // PUERTO DEL SERVIDOR

byte pir = 7; //PIN DIGITAL DEL SENSOR DE MOVIMIENTO
int val = 0;

RestClient client = RestClient(IP, PORT); //SE CREA UN NUEVO CLIENTE REST

void setup() {
  pinMode(pir, INPUT);
  Serial.begin(9600);
  // CONEXION VIA DHCP
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Setup!");
}

String datos = "Mov"; //AQUÍ SE ALMACENARÁ LOS DATOS GENERADOS POR EL SENSOR
String nombre = "SHA"; //NOMBRE QUE SE LE DARÁ A CADA SENSOR
String tipo = "Movimiento"; //DEFINIR MOV O PROX PARA REDUCIR LAS CONSULTAS
//FIN DATOS DE PRUEBA
String response;

void loop(){
  val = digitalRead(pir);
  Serial.println(val);
  if(val == 1){
    datos = "Movimiento Detectado"; //A DATOS LE ASIGNAMOS UN VALO CONFORME A LA DETECCION DE MOV.
  }
  if(val == HIGH){
    response = "";
    client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
    client.setHeader("Content-Type: application/json");
    StaticJsonBuffer<200> jsonBuffer;
    char json[256];
    JsonObject& root = jsonBuffer.createObject();
    root["nombre"] = nombre;
    root["datos"] = datos;
    root["tipoSensor"] = tipo;
    root.printTo(json, sizeof(json));
    Serial.println(json);
    int statusCode = client.post("/centinelaApi/api/registrar/datos-sensores", json, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    Serial.print("Response body from server: ");
    Serial.println(response["Datos"]);
  } else {
    Serial.println("El sensor no está registrando movimiento.");
  }
  delay(1000);
}
