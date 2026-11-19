#include <RH_ASK.h>;
#include <SPI.h>;

RH_ASK rf_driver(2000, 4, 11, 0);

const int LED_PIN = 7;
const int BUZZER_PIN = 8;
const int UNIDAD = 100;
const int mode = 1;

void setup() {
  // put your setup code here, to run once:
  rf_driver.init();
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void senal(int duracion) {
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 800);   
  delay(duracion);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);     
  delay(UNIDAD);
}

void punto() { senal(UNIDAD); }
void raya()  { senal(3 * UNIDAD); }

void reproducirSOS() {
  punto(); punto(); punto();   // S
  delay(2 * UNIDAD);
  raya(); raya(); raya();      // O
  delay(2 * UNIDAD);
  punto(); punto(); punto();   // S
}


void loop() {
  
  if (mode == 0){
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN + 1];
    uint8_t buflen = RH_ASK_MAX_MESSAGE_LEN;

    if (rf_driver.recv(buf, &buflen)) {
      buf[buflen] = '\0';
      Serial.print("Mensaje recibido: ");
      Serial.println((char *)buf);

      if (strcmp((char *)buf, "SOS") == 0) {
        reproducirSOS();
      }
    }
  }

  if (mode == 1){
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN + 1];
    uint8_t buflen = RH_ASK_MAX_MESSAGE_LEN;

    if (rf_driver.recv(buf, &buflen)){
      buf[buflen] = '\0';
      if (strcmp((char *)buf, "1") == 0){
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, 800, 3*UNIDAD);
        Serial.println((char *)buf);
      }    
      else{
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN); 
      Serial.println((char *)buf);    
      }  
    }
  }
}