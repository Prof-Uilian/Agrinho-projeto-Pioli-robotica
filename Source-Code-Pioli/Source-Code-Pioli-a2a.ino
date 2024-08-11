#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "Air_Quality_Sensor.h"
#include <DHT.h>
#include <A2a.h>

Adafruit_BMP280 bmp;
AirQualitySensor sensor(A0);
DHT dht(2, DHT11);
A2a a2a;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial) delay(100); // Espera pela conexão serial
  
  if (!bmp.begin()) {
    Serial.println(F("Falha ao iniciar o sensor BMP280"));
    while (1) delay(10);
  }

  if (!sensor.init()) {
    Serial.println("Falha ao iniciar o sensor de qualidade do ar");
    while (1) delay(10);
  }

  dht.begin();
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25);
  int airQuality = sensor.getValue();

  // Construa um byte array para enviar os dados
  byte dataToSend[6];
  dataToSend[0] = (byte)temperature;
  dataToSend[1] = (byte)pressure;
  dataToSend[2] = (byte)altitude;
  dataToSend[3] = (byte)(airQuality >> 8); // Os dois bytes superiores do valor de airQuality
  dataToSend[4] = (byte)(airQuality & 0xFF); // Os dois bytes inferiores do valor de airQuality

  // Envie os dados para o ESP01 via I2C
  a2a.digitalWireWrite(8, dataToSend, sizeof(dataToSend));
  
  delay(10000); // Espera 10 segundos entre as leituras
}
