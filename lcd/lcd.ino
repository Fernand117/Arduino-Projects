// Incluímos la libreria externa para poder utilizarla
#include <LiquidCrystal.h> // Entre los símbolos <> buscará en la carpeta de librerías configurada

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
#define VELOCIDAD 300 // Velocidad a la que se mueve el texto

// Lo primero is inicializar la librería indicando los pins de la interfaz
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Textos
String texto_fila = "Programarfacil.com";

void setup() {
  // Configuración monitor serie
  Serial.begin(9600);

  // Configuramos las filas y las columnas del LCD en este caso 16 columnas y 2 filas
  lcd.begin(COLS, ROWS);
}

void loop() {

  // Obtenemos el tamaño del texto
  int tam_texto=texto_fila.length();

  // Mostramos entrada texto por la izquierda
  for(int i=tam_texto; i>0 ; i--)
  {
    String texto = texto_fila.substring(i-1);

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
    lcd.print(texto_fila);

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
    lcd.print(texto_fila);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Mostramos salida del texto por la izquierda
  for(int i=1; i<=tam_texto ; i++)
  {
    String texto = texto_fila.substring(i-1);

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
