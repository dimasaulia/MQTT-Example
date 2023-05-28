#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

String SSID = "vivo T1 Pro 5G";
String PWD = "12345678";
const char *MQTT_SERVER = "192.168.226.130";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp", "dimasaulia", "t4np454nd1")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("reminder-A9Cac");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("vivo T1 Pro 5G", "12345678");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
  Serial.println("WORK");
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}
int interval = 5000;
int id = 1;
unsigned long sendTimeStamp = 0;
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (millis() - sendTimeStamp > interval) {
    sendTimeStamp = millis();
    String idStr = String(id);
    int temp = random(34, 40);
    String tempStr = String(temp);
    String payload = "{\"temp\": \"" + tempStr + "\",\"id\": \"GsEgt\"}";
    const char *textChar = payload.c_str();
    client.publish("/bracelet/update/temp/GsEgt", textChar);
    Serial.print("Publish New Topic: ");
    Serial.println(payload);
    id++;
  }

  client.loop();
}
