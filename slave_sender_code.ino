#include <WiFi.h>
#include <esp_now.h>
#include "DHT.h"
 
// DHT Sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// ESP-NOW settings
#define ESPNOW_WIFI_CHANNEL 6
// Sleep interval in microseconds (5 seconds)
#define SLEEP_INTERVAL 5000000
// Structure to send data
typedef struct struct_message {
  float temperature;
  float humidity;
  unsigned long timestamp; // Timestamp field
} struct_message; 
struct_message sensorData; 
// Master MAC address

uint8_t masterAddress[] = {0x08, 0xB6, 0x1F, 0x27, 0x93, 0xFC};  
// Callback function for data sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
} 
// Setup function
void setup() {
  Serial.begin(115200);
  // Initialize DHT sensor
  dht.begin(); 
  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  Serial.println("ESP-NOW Slave with DHT Sensor and Light Sleep");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  } 
  // Register send callback
  esp_now_register_send_cb(onDataSent);
  // Register the master as a peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, masterAddress, 6);
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add master as peer");
    return;
  }
  Serial.println("Master peer added successfully");
} 
// Function to send sensor data
void sendSensorData() {
  // Read temperature and humidity
  sensorData.temperature = dht.readTemperature();
  sensorData.humidity = dht.readHumidity(); 
  // Check if data is valid
  if (isnan(sensorData.temperature) || isnan(sensorData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  // Add timestamp
  sensorData.timestamp = millis();
  // Send data to master
  esp_err_t result = esp_now_send(masterAddress, (uint8_t *)&sensorData, sizeof(sensorData));
  if (result == ESP_OK) {
    Serial.println("Data sent successfully:");
    Serial.print("  Temperature: ");
    Serial.println(sensorData.temperature);
    Serial.print("  Humidity: ");
    Serial.println(sensorData.humidity);
    Serial.print("  Timestamp: ");
    Serial.println(sensorData.timestamp);
  } else {
    Serial.println("Error sending the data");
  }
} 
// Function to enter light sleep
void enterLightSleep() {
  Serial.println("Entering light sleep...");
  esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL); // Set wake-up timer
  esp_light_sleep_start(); // Enter light sleep
  Serial.println("Woke up from light sleep");
} 
void loop() {
  sendSensorData(); // Send sensor data
  enterLightSleep(); // Enter light sleep mode
}

 
