#include <HTTPClient.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi e MQTT
const char* mqtt_server = "broker.emqx.io";
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Pinos
#define rad 34

// Variáveis globais
WiFiClient espClient;
PubSubClient client(espClient);

long lastTime = 0;
const int Msg_Size = 100;
char msg[Msg_Size];

// Variáveis para data/hora
String currentDateTime = "N/A";
const String timezone = "Europe/Lisbon";
const String worldTimeApiUrl = "http://worldtimeapi.org/api/timezone/" + timezone;

void setup_wifi() {
  Serial.println("Iniciando conexão WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado com sucesso!");
}

String getCurrentDateTime() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(worldTimeApiUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      // Parse JSON response
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      String datetime = doc["datetime"].as<String>();
      datetime = datetime.substring(0, 19); // Formato: YYYY-MM-DD HH:MM:SS

      http.end();
      return datetime;
    } else {
      Serial.print("Erro na requisição HTTP: ");
      Serial.println(httpCode);
      http.end();
      return "Erro API";
    }
  } else {
    return "Sem WiFi";
  }
}

void publishData(const char* topic, const char* payload) {
  if (client.publish(topic, payload)) {
    Serial.print("Publicado em ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(payload);
  } else {
    Serial.print("Falha ao publicar em ");
    Serial.println(topic);
  }
}

void reconnect() {
  Serial.print("Conectando ao broker MQTT... ");
  
  String clientId = "AulaIoT-";
  clientId += String(random(0xffff), HEX);

  if (client.connect(clientId.c_str())) {
    Serial.println("Conectado ao broker MQTT!");
    client.publish("IPB/IoT/Projeto/Status", "Dispositivo conectado");
  } else {
    Serial.print("Falhou, rc=");
    Serial.print(client.state());
    Serial.println(" - tentando novamente em 5 segundos...");
    delay(5000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Inicializando...");

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  currentDateTime = getCurrentDateTime();
  Serial.print("Data/hora atual: ");
  Serial.println(currentDateTime);
}

void loop() {
  // Verifica e reconecta WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado! Tentando reconectar...");
    setup_wifi();
  }

  // Verifica e reconecta MQTT
  if (!client.connected()) {
    Serial.println("Desconectado do Broker MQTT! Tentando reconectar...");
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastTime > 2000) {
    lastTime = now;

    // Atualiza e imprime a data/hora
    String datetime = getCurrentDateTime();
    publishData("IPB/IoT/Projeto/DataHora", datetime.c_str());
    // Lê valor do sensor
    int radiancia = analogRead(rad);
    snprintf(msg, Msg_Size, "%d", radiancia);

    publishData("IPB/IoT/Projeto/Radiancia", msg);
  }
}
