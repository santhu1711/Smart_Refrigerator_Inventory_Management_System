🧊 IoT-Based Smart Refrigerator with Intelligent Inventory Management

![Python](https://img.shields.io/badge/Python-3.x-blue)
![ESP32](https://img.shields.io/badge/ESP32-IoT-red)
![OpenCV](https://img.shields.io/badge/OpenCV-Computer%20Vision-green)
![Blynk](https://img.shields.io/badge/Blynk-IoT-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)

An IoT-enabled Smart Refrigerator system that integrates **ESP32**, **RFID authentication**, **Python**, **OpenCV**, **QR code detection**, **Load Cell (HX711)**, **DHT22 temperature monitoring**, and **Blynk Cloud** to provide secure access, real-time inventory tracking, environmental monitoring, and intelligent food management.

---

## 📖 Project Overview

The **IoT-Based Smart Refrigerator with Intelligent Inventory Management** is an innovative IoT solution designed to automate refrigerator inventory management while enhancing food safety and reducing wastage.

The system authenticates users using **RFID cards**, identifies food items through **QR code scanning** with **Python and OpenCV**, measures item weight using a **Load Cell (HX711)**, continuously monitors internal temperature using a **DHT22 sensor**, and synchronizes real-time data with the **Blynk Cloud** for remote monitoring and notifications.

By combining embedded systems, computer vision, cloud connectivity, and automation, the project provides an intelligent and efficient approach to modern food inventory management.

---

## ✨ Key Features

- 🔐 **RFID-Based Authentication** – Allows secure user access using RFID cards before interacting with the system.

- 📦 **QR Code-Based Inventory Management** – Uses Python and OpenCV to scan QR codes for automatically adding and managing food items.

- ⚖️ **Weight Measurement** – Monitors the quantity of stored food using a Load Cell and HX711 amplifier.

- 🌡️ **Temperature Monitoring** – Continuously tracks the refrigerator's internal temperature using the DHT22 sensor.

- ☁️ **Cloud Monitoring with Blynk** – Sends real-time sensor data to the Blynk Cloud for remote monitoring and notifications.

- 📊 **Inventory Logging** – Stores inventory records and user activity using CSV files for future analysis.

- 📺 **LCD Display** – Displays system status, authentication messages, and inventory information in real time.

- 🔄 **ESP32–Python Communication** – Establishes serial communication between the ESP32 microcontroller and the Python application for seamless data exchange.

- 🛒 **Automated Inventory Tracking** – Reduces manual effort by automatically updating inventory information.

- 🚀 **Modular Design** – Designed to allow future integration of AI-based food recommendations, expiry prediction, and mobile applications.

---
# 🛠️ Technology Stack

## Hardware

- ESP32 Development Board
- RFID Module (MFRC522)
- RFID Cards
- USB Camera
- Load Cell + HX711
- DHT22 Temperature Sensor
- 16x2 LCD Display (I2C)
- 7805 Voltage Regulator

---

## Software

- Python
- OpenCV
- Arduino IDE
- ESP32 Arduino Framework
- Blynk IoT Platform
- CSV Data Logging

---

## Communication

- Serial Communication (ESP32 ↔ Python)
- Wi-Fi
- Blynk Cloud

---

## 📂 Repository Structure

```text
Smart_Refrigerator_Inventory_Management_System
│
├── demo/
│   └── Smart_Refrigerator_Demo.mp4
│
├── docs/
│   └── Project_Documentation.pdf
│
├── esp32/
│   └── Smart_Refrigerator.ino
│
├── python/
│   └── smart_refrigerator.py
│
├── images/
│
├── LICENSE
└── README.md
```

---

## 🎥 Project Demonstration

A complete working demonstration of the Smart Refrigerator system is available in the `demo` folder.

---

## ⚙️ How It Works

1. User scans RFID card for authentication.
2. ESP32 verifies the user access.
3. Camera captures food item QR code.
4. Python + OpenCV processes the QR data.
5. Inventory details are updated automatically.
6. Load cell measures quantity changes.
7. DHT22 monitors refrigerator temperature.
8. ESP32 sends sensor data to Blynk Cloud.
9. User can monitor refrigerator status remotely.

---

## 🚀 Installation

### Clone Repository

git clone https://github.com/santhu1711/Smart_Refrigerator_Inventory_Management_System.git

### ESP32

- Install Arduino IDE
- Install ESP32 Board Package
- Install Required Libraries

### Python

pip install -r requirements.txt

python smart_refrigerator.py

---

## 🚀 Future Enhancements

- AI-based food expiry prediction
- Mobile Application
- Voice Assistant Integration
- OCR-based label detection
- Firebase Database
- Email Notifications
- Machine Learning for food recommendations

---

## 📈 Project Outcome

✔ Reduced manual inventory management

✔ Improved food monitoring

✔ Enhanced refrigerator security

✔ Real-time IoT monitoring

✔ Modular architecture for future AI integration

---

## 👨‍💻 Author

**Santhosh S.**

- GitHub: https://github.com/santhu1711
- LinkedIn: https://www.linkedin.com/in/santhosh17/final 
