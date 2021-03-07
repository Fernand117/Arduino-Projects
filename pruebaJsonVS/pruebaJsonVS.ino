// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> // Entre los símbolos <> buscará en la carpeta de librerías configurada
#include <Ethernet.h>
#include <SPI.h>

#include <ArduinoJson.h>
#include "RestClient.h"

#define IP "192.168.1.69" // DIRECCION IP DEL SERVIDOR
#define PORT 80     // PUERTO DEL SERVIDOR

byte pir = 7; //PIN DIGITAL DEL SENSOR DE MOVIMIENTO
int val = 0;

RestClient client = RestClient(IP, PORT); //SE CREA UN NUEVO CLIENTE REST
// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
#define VELOCIDAD 300// Velocidad a la que se mueve el texto

String datos = "Mov"; //AQUÍ SE ALMACENARÁ LOS DATOS GENERADOS POR EL SENSOR
String nombre = "SHA"; //NOMBRE QUE SE LE DARÁ A CADA SENSOR
String tipo = "Movimiento"; //DEFINIR MOV O PROX PARA REDUCIR LAS CONSULTAS
//FIN DATOS DE PRUEBA
String response;

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Textos
String texto_fila = "Programarfacil.com";

void setup() {
  pinMode(pir, INPUT);
  Serial.begin(9600);
  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  lcd.begin(COLS, ROWS);
  // CONEXION VIA DHCP
  texto_fila = "connect to network";
  printMensaje(texto_fila);
  delay(VELOCIDAD);
  texto_fila = client.dhcp();
  printMensaje(texto_fila);
  delay(VELOCIDAD);
  texto_fila = "Setup!";
  printMensaje(texto_fila);
  delay(VELOCIDAD);
}

void loop() {
    val = digitalRead(pir);
    texto_fila = val;
    printMensaje(texto_fila);
    delay(VELOCIDAD);
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
        texto_fila = "Status code from server: ";
        printMensaje(texto_fila);
        delay(VELOCIDAD);
        texto_fila = statusCode;
        printMensaje(texto_fila);
        delay(VELOCIDAD);
        texto_fila = "Response body from server: ";
        printMensaje(texto_fila);
        delay(VELOCIDAD);
        texto_fila = response["Datos"];
        printMensaje(texto_fila);
        delay(VELOCIDAD);
    } else {
        texto_fila = "El sensor no está registrando movimiento.";
        printMensaje(texto_fila);
        delay(VELOCIDAD);
    }
    delay(1000);
}

void printMensaje(String mensaje){

  // Obtenemos el tamaño del texto
  int tam_texto = mensaje.length();

  // Mostramos entrada texto por la izquierda
  for(int i=tam_texto; i>0 ; i--)
  {
    String texto = mensaje.substring(i-1);

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
  for(int i=1; i<=16;i++)
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
  for(int i=16;i>=1;i--)
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
  for(int i=1; i<=tam_texto ; i++)
  {
    String texto = mensaje.substring(i-1);

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
