#include <Wire.h>
#include <MPU6050.h>

#define TRIGGER_PIN_1 2 // Pino de trigger do primeiro sensor
#define ECHO_PIN_1 3    // Pino de echo do primeiro sensor

#define TRIGGER_PIN_2 4 // Pino de trigger do segundo sensor
#define ECHO_PIN_2 5    // Pino de echo do segundo sensor

#define VIBRATOR_PIN 6  // Pino do vibrador

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(VIBRATOR_PIN, OUTPUT);
}

void loop() {
  int angle = mpu.getAngleX();

  if (angle < 120 || angle > 225) {
    long duration_1, distance_1, duration_2, distance_2;

    // Leitura do primeiro sensor
    digitalWrite(TRIGGER_PIN_1, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN_1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN_1, LOW);
    duration_1 = pulseIn(ECHO_PIN_1, HIGH);
    distance_1 = duration_1 * 0.034 / 2;

    // Leitura do segundo sensor
    digitalWrite(TRIGGER_PIN_2, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN_2, LOW);
    duration_2 = pulseIn(ECHO_PIN_2, HIGH);
    distance_2 = duration_2 * 0.034 / 2;

    // Calcula a média das distâncias
    float average_distance = (distance_1 + distance_2) / 2.0;

    Serial.print("Distância média: ");
    Serial.println(average_distance);

    // Ativa o vibrador com base na distância
    if (average_distance < 10) {
      digitalWrite(VIBRATOR_PIN, HIGH);
    } else {
      digitalWrite(VIBRATOR_PIN, LOW);
    }

    delay(500);
  } else {
    digitalWrite(VIBRATOR_PIN, LOW);
  }
}