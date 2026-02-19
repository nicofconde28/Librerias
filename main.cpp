#include <Arduino.h>
#include <TM1638.h>

// Pines (puedes cambiarlos si quieres)
#define STB_PIN 10   // D10
#define CLK_PIN 9    // D9
#define DIO_PIN 8    // D8

TM1638 tm1638(STB_PIN, CLK_PIN, DIO_PIN);

unsigned long previousDisplayMillis = 0;
unsigned long previousKeyMillis = 0;

int conteo = 0;

void setup() {
    Serial.begin(115200);
    
    tm1638.init();
    tm1638.setBrightness(7);   // Brillo máximo

    Serial.println("Arranque del programa");

    // Prueba inicial
    tm1638.displayNumber(12345678);
    delay(2000);
    tm1638.clearDisplay();
}

void loop() {

    // 🔹 Actualizar display cada 500 ms
    if (millis() - previousDisplayMillis >= 500) {
        previousDisplayMillis = millis();
        tm1638.displayNumber(conteo++);
    }

    // 🔹 Leer botones cada 100 ms
    if (millis() - previousKeyMillis >= 100) {
        previousKeyMillis = millis();

        uint8_t keys = tm1638.readKeys();

        if (keys) {
            Serial.print("Keys pressed: ");
            Serial.println(keys, HEX);
        }
    }
}
