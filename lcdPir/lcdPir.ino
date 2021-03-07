#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"

// Definimos las constantes que vamosa utilizar
#define COLS 16       // Columnas del LCD
#define ROWS 2        // Filas del LCD
#define VELOCIDAD 300 // Velocidad a la que se mueve el texto

#define IP "192.168.1.69" // DIRECCION IP DEL SERVIDOR
#define PORT 80           // PUERTO DEL SERVIDOR

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

RestClient client = RestClient(IP, PORT); //SE CREA UN NUEVO CLIENTE REST

byte pir = 7; //PIN DIGITAL DEL SENSOR DE MOVIMIENTO
int val = 0;

// Textos
String texto_fila;          //ESTA VARIABLE RECIBE LOS MENSAJES
String datos = "Mov";       //AQUÍ SE ALMACENARÁ LOS DATOS GENERADOS POR EL SENSOR
String nombre = "SHA";      //NOMBRE QUE SE LE DARÁ A CADA SENSOR
String tipo = "Movimiento"; //DEFINIR MOV O PROX PARA REDUCIR LAS CONSULTAS
String response;            //AQUÍ SE ALMACENA LA RESPUESTA DEL SERVIDOR

void setup()
{
  pinMode(pir, INPUT);

  Serial.begin(9600);

  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  lcd.begin(COLS, ROWS);

  texto_fila = "Conectando con la red";
  printMensaje(texto_fila);

  delay(VELOCIDAD);

  texto_fila = client.dhcp(); //Conectamos con DHCP para asignar IP en la red local
  Serial.println(texto_fila);
  
  printMensaje(texto_fila);

  delay(VELOCIDAD);

  texto_fila = "Conectado!";
  Serial.println(texto_fila);
  
  printMensaje(texto_fila);

  delay(VELOCIDAD);
}

void loop()
{
  val = digitalRead(pir);
  texto_fila = val;
  Serial.println(texto_fila);
  printMensaje(texto_fila);

  if (val == 1)
  {
    datos = "Movimiento detectado";
    Serial.println(datos);
    printMensaje(datos);
  }
  if (val == HIGH)
  {
    printMensaje("Movimiento detectado");

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
    printMensaje(texto_fila);

    texto_fila = statusCode;
    Serial.println(texto_fila);
    printMensaje(texto_fila);

    texto_fila = "Response body from server: ";
    Serial.println(texto_fila);
    printMensaje(texto_fila);

    texto_fila = response;
    Serial.println(texto_fila);
    printMensaje(texto_fila);
  }
  else
  {
    printMensaje("No hay movimiento");
  }
  delay(VELOCIDAD);
}

void printMensaje(String mensaje)
{
  Serial.println("Mensaje enviado: " + mensaje);
  // Obtenemos el tamaño del texto
  int tam_texto = mensaje.length();

  // Mostramos entrada texto por la izquierda
  for (int i = tam_texto; i > 0; i--)
  {
    String texto = mensaje.substring(i - 1);

    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(0, 0);

    // Escribimos el texto
    lcd.print(texto);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Desplazamos el texto hacia la derecha
  for (int i = 1; i <= 16; i++)
  {
    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(i, 0);

    // Escribimos el texto
    lcd.print(mensaje);

    // Esperamos
    delay(VELOCIDAD);
  }
  // Desplazamos el texto hacia la izquierda en la segunda fila
  for (int i = 16; i >= 1; i--)
  {
    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(i, 1);

    // Escribimos el texto
    lcd.print(mensaje);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Mostramos salida del texto por la izquierda
  for (int i = 1; i <= tam_texto; i++)
  {
    String texto = mensaje.substring(i - 1);

    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(0, 1);

    // Escribimos el texto
    lcd.print(texto);

    // Esperamos
    delay(VELOCIDAD);
  }
}