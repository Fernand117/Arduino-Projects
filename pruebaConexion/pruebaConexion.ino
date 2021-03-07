#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
 
void setup()
{
  Serial.begin(9600);
 
  Ethernet.begin(mac, ip);
  server.begin();
 
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
 
void loop()
{
  EthernetClient client = server.available(); 
  if (client)
  {
    Serial.println("new client");
    bool currentLineIsBlank = true;
    String cadena = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        
        // Al recibir linea en blanco, servir página a cliente
        if (c == '\n' && currentLineIsBlank)
        {
          client.println(F("HTTP/1.1 200 OK\nContent-Type: text/html"));
          client.println();
 
          client.println(F("<html>\n<head>\n<title>Luis Llamas</title>\n</head>\n<body>"));
          client.println(F("<div style='text-align:center;'>"));
 
          client.println(F("<h2>Entradas digitales</h2>"));
          for (int i = 0; i < 13; i++)
          {
            client.print("D");
            client.print(i);
            client.print(" = ");
            client.print(digitalRead(i));
            client.println(F("<br />"));
          }
          client.println("<br /><br />");
 
          client.println(F("<h2>Entradas analogicas</h2>"));
          for (int i = 0; i < 7; i++)
          {
            client.println("A");
            client.println(i);
            client.println(" = ");
            client.println(analogRead(i));
            client.println(F("<br />"));
          }
 
          client.println(F("<a href='http://192.168.1.177'>Refrescar</a>"));
          client.println(F("</div>\n</body></html>"));
          break;
        }
        if (c == '\n') 
        {
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          currentLineIsBlank = false;
        }
      }
    }
 
    delay(1);
    client.stop();
  }
}
