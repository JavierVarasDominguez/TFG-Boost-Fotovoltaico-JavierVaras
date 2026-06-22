/*
  PWM seguro para prototipo Boost con ESP32

  Pin PWM: GPIO25 / D25
  Frecuencia: 50 kHz
  Duty inicial: 0 %
  Control por monitor serie

  Comandos:
    start   -> activa PWM al 10 %
    stop    -> apaga PWM
    d 20    -> duty al 20 %
    status  -> muestra estado
*/

#include <Arduino.h>

const int PWM_PIN = 25;
const int PWM_FREQ = 50000;
const int PWM_RESOLUTION = 10;

const float DUTY_START = 10.0;
const float DUTY_MAX = 60.0;

float dutyPercent = 0.0;
bool pwmOn = false;

const int PWM_MAX = (1 << PWM_RESOLUTION) - 1;

void setDuty(float duty) {
  if (duty < 0.0) duty = 0.0;
  if (duty > DUTY_MAX) duty = DUTY_MAX;

  dutyPercent = duty;

  int dutyValue = (dutyPercent / 100.0) * PWM_MAX;
  ledcWrite(PWM_PIN, dutyValue);

  Serial.print("Duty = ");
  Serial.print(dutyPercent);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ledcAttach(PWM_PIN, PWM_FREQ, PWM_RESOLUTION);

  setDuty(0.0);

  Serial.println("PWM listo.");
  Serial.println("Comandos: start | stop | d 20 | status");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "start") {
      pwmOn = true;
      setDuty(DUTY_START);
      Serial.println("PWM activado.");
    }

    else if (cmd == "stop") {
      pwmOn = false;
      setDuty(0.0);
      Serial.println("PWM apagado.");
    }

    else if (cmd == "status") {
      Serial.println("----- ESTADO -----");
      Serial.print("PWM: ");
      Serial.println(pwmOn ? "ON" : "OFF");
      Serial.print("Frecuencia: ");
      Serial.print(PWM_FREQ);
      Serial.println(" Hz");
      Serial.print("Duty actual: ");
      Serial.print(dutyPercent);
      Serial.println(" %");
    }

    else if (cmd.startsWith("d ")) {
      float newDuty = cmd.substring(2).toFloat();

      if (newDuty >= 0.0 && newDuty <= DUTY_MAX) {
        pwmOn = true;
        setDuty(newDuty);
      } else {
        Serial.println("Duty fuera de rango. Maximo permitido: 60 %");
      }
    }

    else {
      Serial.println("Comando no reconocido.");
    }
  }
}