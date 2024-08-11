#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* serverAddress = "cloud.codeexplorers.tech"; // Insira o endereço IP do seu servidor
const int serverPort = 80;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  
  if (!wifiManager.autoConnect("Configurar-Horta")) {
    Serial.println("Falha ao conectar-se e tempo esgotado");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  
  Serial.println("Conectado à rede Wi-Fi!");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (client.connected() && client.available()) {
    String response = client.readStringUntil('\r');
    Serial.println(response);
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    sendJSONToServer(data);
  }
}

void sendJSONToServer(String jsonData) {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Falha na conexão com o servidor");
    return;
  }

  StaticJsonDocument<200> jsonDocument;
  DeserializationError error = deserializeJson(jsonDocument, jsonData);
  
  if (error) {
    Serial.print("deserializeJson() falhou: ");
    Serial.println(error.c_str());
    return;
  }

  String postRequest = "POST /insert_data.php HTTP/1.1\r\n"; // Caminho correto para o arquivo PHP
  postRequest += "Host: ";
  postRequest += serverAddress;
  postRequest += "\r\n";
  postRequest += "Content-Type: application/json\r\n";
  postRequest += "Content-Length: ";
  postRequest += measureJson(jsonDocument);
  postRequest += "\r\n\r\n";
  postRequest += jsonData;

  client.print(postRequest);
  delay(500);

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}
