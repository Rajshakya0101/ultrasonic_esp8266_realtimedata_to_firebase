# Ultrasonic Sensor with ESP8266 and Firebase

This repository demonstrates how to use an ESP8266 microcontroller to read distance measurements from an ultrasonic sensor and send the data to a Firebase Realtime Database.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Wiring Diagram](#wiring-diagram)
- [Setup Instructions](#setup-instructions)
- [Code Explanation](#code-explanation)
- [Usage](#usage)
- [License](#license)
- [References](#references)

## Introduction
This project demonstrates how to use an ultrasonic sensor to measure distances and upload the data to Firebase for remote monitoring using an ESP8266 microcontroller.

## Features
- Measure distances using an ultrasonic sensor.
- Send data to Firebase Realtime Database.
- Monitor distance data in real-time from any Firebase-supported platform.

## Hardware Requirements
- ESP8266 (e.g., NodeMCU)
- Ultrasonic sensor (e.g., HC-SR04)
- Jumper wires
- Breadboard

## Software Requirements
- Arduino IDE
- Firebase ESP8266 Client Library

## Wiring Diagram

| Component     | ESP8266 Pin |
|---------------|-------------|
| Ultrasonic Trig | D1 (GPIO5) |
| Ultrasonic Echo | D2 (GPIO4) |
| VCC           | 3.3V        |
| GND           | GND         |

## Setup Instructions

### 1. Install Arduino IDE
- Download and install the Arduino IDE from [Arduino](https://www.arduino.cc/en/software).

### 2. Install Required Libraries
- Open Arduino IDE.
- Go to **Sketch** -> **Include Library** -> **Manage Libraries**.
- Search for `Firebase ESP8266 Client` and install it.

### 3. Firebase Setup
- Go to the [Firebase Console](https://console.firebase.google.com/).
- Create a new project.
- In the project settings, enable the Realtime Database.
- Note down your Firebase **API Key** and **Database URL**.

### 4. Configure Wi-Fi and Firebase Credentials
- Update the following placeholders in the code:
  ```cpp
  #define WIFI_SSID "<Your Wi-Fi SSID>"
  #define WIFI_PASSWORD "<Your Wi-Fi Password>"
  #define API_KEY "<Your Firebase API Key>"
  #define DATABASE_URL "<Your Firebase Database URL>"
  ```

### 5. Upload the Code
- Connect your ESP8266 to your computer.
- Select the appropriate board and port in the Arduino IDE (**Tools** -> **Board** -> **NodeMCU 1.0** or your ESP8266 variant).
- Upload the code to the ESP8266.

## Code Explanation

### 1. Ultrasonic Sensor
The ultrasonic sensor calculates distance by measuring the time it takes for an ultrasonic pulse to travel to an object and back. The formula to calculate distance is:

```
Distance (cm) = (Time / 2) / 29.1
```

### 2. Firebase Integration
The ESP8266 connects to Firebase using the Firebase ESP8266 Client Library. Data is sent to the database using:
```cpp
Firebase.RTDB.setFloat(&fbdo, "Ultrasonic/distance", distance);
```

### 3. Wi-Fi Connection
The ESP8266 connects to the Wi-Fi network using:
```cpp
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
```

## Usage
1. Power the ESP8266.
2. Open the Serial Monitor in the Arduino IDE to view connection and distance status.
3. Check your Firebase Realtime Database to see the `Ultrasonic/distance` node being updated.

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## References
- [Firebase ESP8266 Client Library](https://github.com/mobizt/Firebase-ESP8266)
- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
