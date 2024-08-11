#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include "Air_Quality_Sensor.h"
#include <DHT11.h>
#include <ArduinoJson.h>

Adafruit_BMP280 bmp;
AirQualitySensor sensor(A0);
DHT11 dht11(2);

void setup() {
  Serial.begin(9600);
  while (!Serial); // Aguarda a conexão serial
  Serial.println(F("Iniciando testes..."));

  // Inicialização do sensor BMP280
  unsigned status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  if (status == 0) {
    Serial.println(F("Erro ao inicializar o sensor BMP280. Verifique a conexão!"));
    while (1);
  }
  Serial.println(F("Sensor BMP280 inicializado com sucesso!"));
  
  // Inicialização do sensor de qualidade do ar
  if (sensor.init()) {
    Serial.println("Sensor de qualidade do ar pronto.");
  } else {
    Serial.println("Erro ao inicializar o sensor de qualidade do ar!");
  }
}

void loop() {
  // Realiza leituras e envia os dados
  sendData();
  
  // Aguarda 2 segundos antes da próxima leitura
  delay(60000);
}

void sendData() {
  // Cria um objeto JSON
  StaticJsonDocument<200> jsonDocument;
  
  // Adiciona os dados ao objeto JSON
  jsonDocument["T2"] = readDHTTemperature();
  jsonDocument["U"] = readDHTHumidity();
  jsonDocument["T"] = bmp.readTemperature();
  jsonDocument["P"] = bmp.readPressure();
  jsonDocument["alt"] = bmp.readAltitude(1013.25);
  jsonDocument["air"] = invertAirQualityValue(sensor.getValue()); // Inverte o valor do sensor
  
  // Converte o objeto JSON em uma string
  String jsonString;
  serializeJson(jsonDocument, jsonString);
  
  // Envia a string JSON via Serial
  Serial.println(jsonString);
}

float readDHTTemperature() {
  int temperature = 0;
  int humidity = 0;
  dht11.readTemperatureHumidity(temperature, humidity);
  return temperature;
}

float readDHTHumidity() {
  int temperature = 0;
  int humidity = 0;
  dht11.readTemperatureHumidity(temperature, humidity);
  return humidity;
}

int invertAirQualityValue(int originalValue) {
  return 1000 - originalValue; // Inverte o valor do sensor de qualidade do ar
}
