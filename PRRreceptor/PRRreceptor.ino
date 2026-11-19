#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver(2000, 4, 11, 10);
const unsigned long VENTANA_MS = 5000;
const int ESPERADOS = 25;               // 10 s / 200 ms

int recibidos = 0;
unsigned long inicio = 0;

void setup() {
  Serial.begin(9600);
  rf_driver.init();
  inicio = millis();
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen) && buflen == 1) recibidos++;

  if (millis() - inicio >= VENTANA_MS) {
    Serial.print(recibidos);
    Serial.print(",");
    Serial.println(ESPERADOS);
    recibidos = 0;
    inicio = millis();
  }
}