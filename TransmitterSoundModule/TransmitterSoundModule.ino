#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;  // TX en pin 12 (por defecto)

const int MIC_PIN = A0;
const int VENTANA_MS = 50;   // Cada cuanto se envia un resumen
const int UMBRAL = 15;       // Amplitud minima para considerar que hay sonido
const int HISTERESIS = 8;    // Evita contar ruido como ciclos

int centro = 512;            // Nivel medio de la señal (se actualiza solo)

void setup() {
  Serial.begin(9600);
  if (!rf_driver.init()) {
    Serial.println("Error al iniciar el modulo RF");
  }
}

void loop() {
  int minimo = 1023, maximo = 0;
  long suma = 0;
  int n = 0;
  int ciclos = 0;
  bool arriba = false;

  unsigned long inicio = millis();
  while (millis() - inicio < VENTANA_MS) {
    int v = analogRead(MIC_PIN);
    if (v < minimo) minimo = v;
    if (v > maximo) maximo = v;
    suma += v;
    n++;

    if (!arriba && v > centro + HISTERESIS) {
      arriba = true;
      ciclos++;                       // Un cruce ascendente = un ciclo
    } else if (arriba && v < centro - HISTERESIS) {
      arriba = false;
    }
  }
  centro = suma / n;

  int amplitud = maximo - minimo;
  int frecuencia = 0;
  if (amplitud >= UMBRAL) {
    frecuencia = ciclos * (1000 / VENTANA_MS);   // Hz aproximados
  }

  uint8_t paquete[2];
  paquete[0] = constrain(frecuencia / 10, 0, 255);   // Hz / 10 (hasta 2550 Hz)
  paquete[1] = constrain(amplitud / 4, 0, 255);

  rf_driver.send(paquete, 2);
  rf_driver.waitPacketSent();

  Serial.print("Frecuencia: ");
  Serial.print(frecuencia);
  Serial.print(" Hz | Amplitud: ");
  Serial.println(amplitud);
}