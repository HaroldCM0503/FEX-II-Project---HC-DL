#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver(2000, 4, 12, 10);   // velocidad, RX, TX, PTT

const int LED_PIN = 7;
const int BUZZER_PIN = 8;
const unsigned long TIMEOUT_MS = 300;   // Silencio si no llegan paquetes

unsigned long ultimoPaquete = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  if (!rf_driver.init()) {
    Serial.println("Error al iniciar el modulo RF");
  }
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen) && buflen == 2) {
    ultimoPaquete = millis();
    int frecuencia = buf[0] * 10;
    int amplitud = buf[1] * 4;

    if (frecuencia > 0) {
      tone(BUZZER_PIN, frecuencia);
      digitalWrite(LED_PIN, HIGH);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
    }

    Serial.print("Frecuencia: ");
    Serial.print(frecuencia);
    Serial.print(" Hz | Amplitud: ");
    Serial.println(amplitud);
  }

  // Si se pierde la señal, silencio
  if (millis() - ultimoPaquete > TIMEOUT_MS) {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }
}