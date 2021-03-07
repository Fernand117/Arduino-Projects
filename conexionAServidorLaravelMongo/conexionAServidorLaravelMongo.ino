#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0xF6, 0xFF};
byte ip[] = {192, 168, 1, 120};
byte gateway[] = {192,168,1,254};

EthernetClient client;

String datos = "123";
String nombre = "SHA";
String tipo = "Movimiento";

void setup()
{
  Ethernet.begin(mac, ip, gateway);
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  delay(1000);
  delay(1000);
  Serial.println("connecting...");
}

void loop()
{
  Serial.println(client.connect("http://192.168.1.69/centinelaApi/", 80));
  if (client.connect("http://192.168.1.69/centinelaApi/", 80)){
    Serial.println("Sending to Server: ");
    client.println("POST /api/registrar/datos-sensores HTTP/1.1");
    Serial.print("POST /api/registrar/datos-sensores HTTP/1.1");
    client.println("Host: https://36aabf915f3c.ngrok.io");
    client.println("Content-Type: multipart/form-data");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Disposition: Content-Type: text/plain;form-data; name=\"nombre\"\r\n\r\n");
    client.print(nombre + "\r\n");
    client.print("Content-Disposition: Content-Type: text/plain;form-data; name=\"datos\"\r\n\r\n");
    client.print(datos + "\r\n");
    client.print("Content-Disposition: Content-Type: text/plain;form-data; name=\"tipoSensor\"\r\n\r\n");
    client.print(tipo + "\r\n");
    client.println();
  }
  else{
    Serial.println("Cannot connect to Server");
  }
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}
