#include <SPI.h>
#include <RH_ASK.h>  // Biblioteca para RF 433 MHz
#include <Wire.h>

// Pines del joystick
#define JOY_X 34  // Eje X
#define JOY_Y 35  // Eje Y
#define JOY_SW 32 // Botón del joystick

// Pines del módulo RF 433 MHz
#define PIN_RF_TX 14 // Pin de transmisión RF

// Crear instancia del objeto de transmisión RF 433 MHz
RH_ASK rf_driver(2000, PIN_RF_TX, 0, 0);  // 2000 bps, pin de transmisión, sin interrupciones

void setup() {
  // Configuración de pines
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  pinMode(JOY_SW, INPUT_PULLUP);

  // Inicializar la comunicación RF
  if (!rf_driver.init()) {
    Serial.println("Error en la inicialización del módulo RF.");
    while (1);
  }

  Serial.begin(9600); // Para depuración
}

void loop() {
  // Leer valores del joystick
  int xValue = analogRead(JOY_X);
  int yValue = analogRead(JOY_Y);
  bool buttonPressed = !digitalRead(JOY_SW);

  // Mapeo de los valores a un rango de -100 a 100
  int mappedX = map(xValue, 0, 4095, -100, 100);
  int mappedY = map(yValue, 0, 4095, -100, 100);

  // Crear el mensaje que será enviado
  char message[20];
  sprintf(message, "X:%d,Y:%d,B:%d", mappedX, mappedY, buttonPressed);

  // Enviar el mensaje por RF
  rf_driver.send((uint8_t *)message, strlen(message));
  rf_driver.waitPacketSent();

  // Depuración por el monitor serie
  Serial.println(message);

  delay(100); // Enviar cada 100 ms
}