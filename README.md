# ESP32 Multi-Node Sensor Network Using ESP32, ESP-NOW, Node-Red & MQTT


## Table of Contents

1. [Introduction](#1-introduction)  
2. [System Architecture](#2-system-architecture)  
3. [Hardware Implementation](#3-hardware-implementation)  
4. [Software Implementation](#4-software-implementation)  
5. [Installation Guide](#5-installation-guide)  
6. [Operation Guide](#6-operation-guide)  
7. [Performance Analysis](#7-performance-analysis)  
8. [Testing and Validation](#8-testing-and-validation)  
9. [Troubleshooting](#9-troubleshooting)  
10. [Future Enhancements](#10-future-enhancements)  
11. [Technical Specifications](#11-technical-specifications)  
12. [Appendices](#12-appendices)

---

## 1. Introduction

The **ESP32 Temperature Monitoring System** provides a robust solution for environmental monitoring. By leveraging ESP32 microcontrollers, DHT11 sensors, and technologies like ESP-NOW, MQTT, and Node-RED, this system ensures **real-time, reliable data collection and visualization**. The nodes are powered by USB ports, making it highly practical during development and testing.

---

## 2. System Architecture

The architecture comprises **four ESP32 nodes**:
- **2 Sender Nodes**: Equipped with DHT11 sensors, gather temperature and humidity data at configurable intervals.
- **1 Receiver Node**: Aggregates data from senders using ESP-NOW and relays it via I2C.
- **1 Bridge Node**: Publishes data to an MQTT broker for remote access and integration with Node-RED.

This hierarchical structure ensures **reliable, scalable communication** for both local and remote monitoring.  
*(See Figure 1.1 in documentation for visual details.)*

---

## 3. Hardware Implementation

### Hardware Components:
- **Sender Nodes**: DHT11 sensors connected to ESP32 GPIO4 pins.
- **Receiver Node**: Handles ESP-NOW reception and I2C communication.
- **Bridge Node**: Publishes aggregated data via WiFi to MQTT.

### Key Pin Configurations:
- **DHT11 Connections**: VCC, GND, Data to GPIO4.
- **I2C Communication**: GPIO21 (SDA) & GPIO22 (SCL).

All components are USB-powered, ensuring stability during development.

---

## 4. Software Implementation

### Features:
1. **Sender Nodes**: Capture sensor data, package it, and transmit using ESP-NOW.
2. **Receiver Node**: Collects data from multiple senders and forwards via I2C.
3. **Bridge Node**: Handles MQTT publishing and WiFi connectivity.
4. **Node-RED**: Provides an intuitive dashboard for **data visualization** and **alert configuration**.

All firmware includes error handling, configurable sampling intervals, and robust data validation.  
*(See Figures 4.1 & 4.2 for code examples.)*

---

## 5. Installation Guide

### Steps:
1. **Arduino IDE Setup**:
   - Install ESP32 board support.
   - Add libraries: DHT Sensor, ESP-NOW, PubSubClient.
2. **Node-RED Setup**:
   - Install Node.js, NPM, and necessary Node-RED packages (dashboard, MQTT).
3. **MQTT Broker Configuration**:
   - Set connection parameters, security, and topic structure.
4. **System Configuration**:
   - Configure WiFi credentials, MQTT broker details, and sampling intervals.

Perform functionality tests after setup.

---

## 6. Operation Guide

### Sequence:
1. Power up nodes in this order:
   - **Bridge Node → Receiver Node → Sender Nodes**.
2. Monitor real-time data on the Node-RED dashboard with graphs and alerts.
3. Use the dashboard to configure thresholds and notification preferences.

### Maintenance:
- Verify sensor connectivity and data flow.
- Use built-in error logging and diagnostic tools for troubleshooting.

---

## 7. Performance Analysis

### Key Metrics:
- **Temperature Accuracy**: ±0.5°C.
- **Communication Success Rate**: 99.5%.
- **Alert Response Time**: <1 second.

### Power Efficiency:
- **Duty Cycle**: ~1.19%.
- **Average Current**: ~0.234mA.
- **Battery Life (Theoretical)**: ~267 days (with a 1500mAh battery).

### Reliability:
- Stable operation during continuous monitoring.
- Minimal latency in data transmission.

---

## 8. Testing and Validation

### Tests Conducted:
1. **Unit Tests**: Verified each node and sensor.
2. **Integration Tests**: Checked ESP-NOW, I2C, and MQTT communication.
3. **System-Level Validation**: Ensured
