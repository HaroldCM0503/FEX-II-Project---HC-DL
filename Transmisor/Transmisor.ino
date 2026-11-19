#include <RH_ASK.h>;
#include <SPI.h>;

RH_ASK rf_driver;

int mode = 2; //0 = SOS ; 1 = Pulsador Morse (int Out) ; 2 = Pulsador Morse (directo)
int t1 = 0;
int t0 = 0;
int contador = 0;
char *out = "0";
int umbral = 200; //ms

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  Serial.begin(9600);
  pinMode(8, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mode == 2) {
    if (digitalRead(8) == 1) {
      out = "1";    
        }
    else {
      out = "0";
    }
    Serial.print("out= ");
    Serial.println(out);
    Serial.println(rf_driver.send((uint8_t *)out, 1));
    rf_driver.waitPacketSent();
    delay(5);    
  }
  if (mode == 1) {
    t1 = millis();
    if (t1 - t0 == 1) {
      if (digitalRead(8) == HIGH) {
        contador = contador + 1;
      }
      else {
        contador = 0;
      }
      t0 = t1;
    }
    if (contador >= umbral) {
      out = 2;
    }
    else if (contador < umbral && contador > 0) {
      out = 1;
    }
    else if (contador == 0) {
      out = 0;
    }
    Serial.println(rf_driver.send((uint8_t)out, 1));
    rf_driver.waitPacketSent();
    delay(50);
  }
  if (mode == 0) {
    const char *msg = "SOS";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(3500);
  }
}
