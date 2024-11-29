#include <SPI.h>
#include <RH_ASK.h>  // Biblioteca para RF 433 MHz
#include <Wire.h>

// Pines del módulo RF 433 MHz
#define PIN_RF_RX 14 // Pin de recepción RF

// Pines del puente H para motores
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

// Crear instancia del objeto receptor RF 433 MHz
RH_ASK rf_driver(2000, 0, PIN_RF_RX, 0);  // 2000 bps, sin interrupciones, pin de recepción

void setup() {
  // Configuración de los pines de los motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Inicializar la comunicación RF
  if (!rf_driver.init()) {
    Serial.println("Error en la inicialización del módulo RF.");
    while (1);
  }

  Serial.begin(9600); // Para depuración
}

void loop() {
  uint8_t buf[20];
  uint8_t len = sizeof(buf);

  // Verificar si se ha recibido un mensaje
  if (rf_driver.recv(buf, &len)) {
    // Convertir el buffer a un mensaje legible
    buf[len] = '\0';  // Asegurar terminación de cadena
    String message = String((char *)buf);

    // Parsear el mensaje recibido
    int xValue = 0, yValue = 0, button = 0;
    sscanf(message.c_str(), "X:%d,Y:%d,B:%d", &xValue, &yValue, &button);

    // Depuración por el monitor serie
    Serial.println(message);

    // Control de motores según los valores recibidos
    if (yValue > 20) { // Adelante
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if (yValue < -20) { // Atrás
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else if (xValue > 20) { // Derecha
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    } else if (xValue < -20) { // Izquierda
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else { // Detener
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }
}