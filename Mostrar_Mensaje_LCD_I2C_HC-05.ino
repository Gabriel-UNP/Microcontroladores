#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String mensaje = "";
String mensajeAnterior = "";
String mensajeDesplazado = "";
int scrollPos = 0;
unsigned long ultimaActualizacion = 0;
const int intervaloScroll = 500; // Tiempo entre desplazamientos (ms)

void setup() {
  Serial.begin(9600); // Comunicación con el HC-05
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Esperando texto");
}

void loop() {
  // Si hay un nuevo mensaje por Bluetooth
  if (Serial.available()) {
    mensaje = Serial.readStringUntil('\n'); // Leer hasta salto de línea
    mensaje.trim();

    if (mensaje.length() > 0) {
      mensajeAnterior = mensaje;
      mensajeDesplazado = mensajeAnterior + "                "; // 16 espacios
      scrollPos = 0;
      lcd.clear();
    }
  }

  // Si hay un mensaje para mostrar
  if (mensajeAnterior.length() > 0) {
    // Controlar tiempo de actualización
    if (millis() - ultimaActualizacion >= intervaloScroll) {
      ultimaActualizacion = millis();

      lcd.setCursor(0, 0);
      lcd.print(mensajeDesplazado.substring(scrollPos, scrollPos + 16));

      scrollPos++;
      if (scrollPos > mensajeDesplazado.length() - 16) {
        scrollPos = 0; // Reiniciar desplazamiento solo después de recorrer todo
      }
    }
  }
}
