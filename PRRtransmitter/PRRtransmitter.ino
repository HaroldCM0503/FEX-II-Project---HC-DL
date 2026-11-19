#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;
uint8_t contador = 0;
unsigned long proximo = 0;

void setup() { rf_driver.init(); }

void loop() {
  if (millis() >= proximo) {
    proximo += 200;                       // 5 paquetes por segundo, fijos
    rf_driver.send(&contador, 1);
    rf_driver.waitPacketSent();
    contador++;
  }
}