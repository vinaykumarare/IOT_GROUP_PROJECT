#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>

// Wi-Fi credentials
const char* ssid = "DESKTOP-UV40VMN 6206";        
const char* password = "5:5n100H";                

// MQTT broker details
const char* mqtt_server = "broker.mqtt.cool";
const int mqtt_port = 1883;

// MQTT Client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Structure to receive ESP-NOW data
typedef struct sensor_data {
    float temperature;
    float humidity;
} sensor_data_t;

// Received data storage
sensor_data_t receivedDataClient1;
sensor_data_t receivedDataClient2;

// Flags for new data
bool hasNewDataClient1 = false;
bool hasNewDataClient2 = false;

// Communication intervals (in milliseconds)
unsigned long client1Interval = 5000; // 5 seconds
unsigned long client2Interval = 5000; // 5 seconds
unsigned long lastClient1Publish = 0;
unsigned long lastClient2Publish = 0;

// Sampling intervals (in milliseconds)
unsigned long client1SamplingInterval = 3000; // 3 seconds
unsigned long client2SamplingInterval = 3000; // 3 seconds
unsigned long lastClient1Sample = 0;
unsigned long lastClient2Sample = 0;

// Temperature thresholds for alerts
float client1TempLowThreshold = 20.0; // Example: 20.0°C
float client1TempHighThreshold = 30.0; // Example: 30.0°C
float client2TempLowThreshold = 18.0; // Example: 18.0°C
float client2TempHighThreshold = 28.0; // Example: 28.0°C;

// Delay between publishing data for Client 1 and Client 2
unsigned long interClientDelay = 1000; // 1 second

// Function to initialize Wi-Fi connection
void setupWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// Function to reconnect to the MQTT broker if disconnected
void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect("ESP32Client")) { // Client ID for MQTT
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

// ESP-NOW callback function to receive data
void OnDataRecv(const esp_now_recv_info_t* recvInfo, const uint8_t* incomingData, int len) {
    sensor_data_t tempData;
    memcpy(&tempData, incomingData, sizeof(tempData));

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
             recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);

    Serial.print("Received MAC Address: ");
    Serial.println(macStr);

    if (strcmp(macStr, "08:B6:1F:29:D0:D4") == 0) { // MAC Address for Client 1
        receivedDataClient1 = tempData;
        hasNewDataClient1 = true;
    } else if (strcmp(macStr, "08:B6:1F:29:D1:34") == 0) { // MAC Address for Client 2
        receivedDataClient2 = tempData;
        hasNewDataClient2 = true;
    }
}

void setup() {
    Serial.begin(115200);

    // Setup Wi-Fi
    setupWiFi();

    // Setup MQTT
    mqttClient.setServer(mqtt_server, mqtt_port);

    // Initialize ESP-NOW
    WiFi.mode(WIFI_STA); // Set to Station mode
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);

    Serial.print("ESP-NOW Initialized. MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop(); // Process MQTT events

    unsigned long currentTime = millis();

    // Sampling for Client 1
    if (currentTime - lastClient1Sample >= client1SamplingInterval) {
        lastClient1Sample = currentTime;
        // Simulate data sampling for Client 1
        Serial.println("Sampling data for Client 1...");
    }

    // Handle data for Client 1
    if (hasNewDataClient1) {
        hasNewDataClient1 = false;

        // Check thresholds and publish alerts
        if (receivedDataClient1.temperature < client1TempLowThreshold) {
            char alertPayload[50];
            snprintf(alertPayload, sizeof(alertPayload), "Temperature below threshold: %.2f", receivedDataClient1.temperature);
            mqttClient.publish("esp32/client1/alerts", alertPayload);
            Serial.print("Client 1 Alert Published: ");
            Serial.println(alertPayload);
        } else if (receivedDataClient1.temperature > client1TempHighThreshold) {
            char alertPayload[50];
            snprintf(alertPayload, sizeof(alertPayload), "Temperature above threshold: %.2f", receivedDataClient1.temperature);
            mqttClient.publish("esp32/client1/alerts", alertPayload);
            Serial.print("Client 1 Alert Published: ");
            Serial.println(alertPayload);
        }

        // Publish temperature and humidity at intervals
        if (currentTime - lastClient1Publish >= client1Interval) {
            lastClient1Publish = currentTime;

            char tempPayload[50];
            snprintf(tempPayload, sizeof(tempPayload), "%.2f", receivedDataClient1.temperature);
            mqttClient.publish("esp32/client1/temperature", tempPayload);
            Serial.print("Client 1 Temperature Published: ");
            Serial.println(tempPayload);

            char humPayload[50];
            snprintf(humPayload, sizeof(humPayload), "%.2f", receivedDataClient1.humidity);
            mqttClient.publish("esp32/client1/humidity", humPayload);
            Serial.print("Client 1 Humidity Published: ");
            Serial.println(humPayload);
        }
    }

    // Add delay between Client 1 and Client 2 publishing
    delay(interClientDelay);

    // Sampling for Client 2
    if (currentTime - lastClient2Sample >= client2SamplingInterval) {
        lastClient2Sample = currentTime;
        // Simulate data sampling for Client 2
        Serial.println("Sampling data for Client 2...");
    }

    // Handle data for Client 2
    if (hasNewDataClient2) {
        hasNewDataClient2 = false;

        // Check thresholds and publish alerts
        if (receivedDataClient2.temperature < client2TempLowThreshold) {
            char alertPayload[50];
            snprintf(alertPayload, sizeof(alertPayload), "Temperature below threshold: %.2f", receivedDataClient2.temperature);
            mqttClient.publish("esp32/client2/alerts", alertPayload);
            Serial.print("Client 2 Alert Published: ");
            Serial.println(alertPayload);
        } else if (receivedDataClient2.temperature > client2TempHighThreshold) {
            char alertPayload[50];
            snprintf(alertPayload, sizeof(alertPayload), "Temperature above threshold: %.2f", receivedDataClient2.temperature);
            mqttClient.publish("esp32/client2/alerts", alertPayload);
            Serial.print("Client 2 Alert Published: ");
            Serial.println(alertPayload);
        }

        // Publish temperature and humidity at intervals
        if (currentTime - lastClient2Publish >= client2Interval) {
            lastClient2Publish = currentTime;

            char tempPayload[50];
            snprintf(tempPayload, sizeof(tempPayload), "%.2f", receivedDataClient2.temperature);
            mqttClient.publish("esp32/client2/temperature", tempPayload);
            Serial.print("Client 2 Temperature Published: ");
            Serial.println(tempPayload);

            char humPayload[50];
            snprintf(humPayload, sizeof(humPayload), "%.2f", receivedDataClient2.humidity);
            mqttClient.publish("esp32/client2/humidity", humPayload);
            Serial.print("Client 2 Humidity Published: ");
            Serial.println(humPayload);
        }
    }

    delay(100); // 
}
