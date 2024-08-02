#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <ctime>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

char ts[20]; // Changed: Store Unix timestamp as a string
float h;
float t;
float gyroX;
float gyroY;
float gyroZ;
float accelX;
float accelY;
float accelZ;

DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu6050(Wire);
long timer = 0;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset = 25200;
const int   daylightOffset = 0;
WiFiUDP udp;
NTPClient timeClient(udp, ntpServer, gmtOffset, daylightOffset);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["timestamp"] = ts; // Changed: Send Unix timestamp as a string
  doc["humidity"] = static_cast<int>(h);
  doc["temperature"] = static_cast<int>(t);
  doc["gyroX"] = static_cast<int>(gyroX);
  doc["gyroY"] = static_cast<int>(gyroY);
  doc["gyroZ"] = static_cast<int>(gyroZ);
  doc["accelX"] = static_cast<int>(accelX);
  doc["accelY"] = static_cast<int>(accelY);
  doc["accelZ"] = static_cast<int>(accelZ);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup()
{
  Serial.begin(115200);
  connectAWS();
  dht.begin();
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  timeClient.begin();
  timeClient.update();
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  mpu6050.update();
  gyroX = mpu6050.getGyroX();
  gyroY = mpu6050.getGyroY();
  gyroZ = mpu6050.getGyroZ();
  accelX = mpu6050.getAccX();
  accelY = mpu6050.getAccY();
  accelZ = mpu6050.getAccZ();
  timeClient.update();
  time_t timestamp = timeClient.getEpochTime();
  
  // Convert the Unix timestamp to a string and store it in ts
  snprintf(ts, sizeof(ts), "%ld", timestamp);

  if (isnan(h) || isnan(t))  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Timestamp: ");
  Serial.print(ts);
  Serial.print(F(" || Humidity: "));
  Serial.print(h);
  Serial.print(F("% || Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(" || GyroX: ");
  Serial.print(gyroX);
  Serial.print(" || GyroY: ");
  Serial.print(gyroY);
  Serial.print(" || GyroZ: ");
  Serial.print(gyroZ);
  Serial.print(" || AccelX: ");
  Serial.print(accelX);
  Serial.print(" || AccelY: ");
  Serial.print(accelY);
  Serial.print(" || AccelZ: ");
  Serial.println(accelZ);

  publishMessage();
  client.loop();
  delay(10000);
}
