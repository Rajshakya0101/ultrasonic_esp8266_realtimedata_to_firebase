#include <Arduino.h>
#include <ESP8266WiFi.h>// Include the ESP8266 Wi-Fi library
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Define pins for the ultrasonic sensor
#define TRIG_PIN D1 // GPIO5 for Trigger Pin
#define ECHO_PIN D2 // GPIO4 for Echo Pin

// Insert your network credentials
#define WIFI_SSID "<Your_Wifi_name>"  // Replace with your Wi-Fi SSID
#define WIFI_PASSWORD "<Your_Wifi_Password>"  // Replace with your Wi-Fi Password

// Insert Firebase project API Key
#define API_KEY "<Web_API_Key>"  // Replace with your Firebase Web API Key

// Insert Firebase Database URL (without https://)
#define DATABASE_URL "<Realtime_database_URL>"  // Replace with your Firebase Database URL

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;  // Firebase authentication object
FirebaseConfig config;  // Firebase configuration object

unsigned long sendDataPrevMillis = 0;  // Stores the last time data was sent to Firebase
bool signupOK = false;  // Boolean to check if Firebase anonymous sign-in is successful

void setup(){
  Serial.begin(115200);  // Start the Serial Monitor with a baud rate of 115200

  // Configure ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);  // Set the Trigger pin as output
  pinMode(ECHO_PIN, INPUT);  // Set the Echo pin as input

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Start connecting to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){  // Wait until Wi-Fi is connected
    Serial.print(".");
    delay(300);  // Delay for connection attempts
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());  // Print the local IP address
  Serial.println();

  // Firebase setup
  config.api_key = API_KEY;  // Set Firebase API Key
  config.database_url = DATABASE_URL;  // Set Firebase Database URL

  // Sign up anonymously to Firebase
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase SignUp OK");
    signupOK = true;  // Set sign-up success to true
  } else {
    Serial.printf("Firebase SignUp Failed: %s\n", config.signer.signupError.message.c_str());  // Print error message
  }

  config.token_status_callback = tokenStatusCallback;  // Set callback for token generation status

  Firebase.begin(&config, &auth);  // Initialize Firebase connection
  Firebase.reconnectWiFi(true);  // Reconnect to Wi-Fi automatically if disconnected
}

void loop(){
  long duration, distance;  // Variables to store duration and calculated distance

  // Check if Firebase is ready and if enough time has passed to send data
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();  // Update the last data sent timestamp

    // Send a 10us pulse to the Trigger pin
    digitalWrite(TRIG_PIN, LOW);  // Ensure Trigger pin is low
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);  // Set Trigger pin high for 10 microseconds
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);  // Set Trigger pin back to low

    // Read the Echo pin duration
    duration = pulseIn(ECHO_PIN, HIGH);  // Measure the time the Echo pin is high

    // Calculate distance in centimeters
    distance = (duration / 2) * 0.0344;  // Formula to calculate distance

    // Check if the distance reading is valid
    if (isnan(distance)) {
      Serial.println("Failed to read from Ultrasonic sensor!");  // Print error message
      return;  // Exit the function if reading fails
    }

    // Send distance data to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "Ultrasonic/distance", distance)){
      Serial.println("Ultrasonic sensor data sent to Firebase successfully");  // Confirm data sent
    } else {
      Serial.println("Failed to send sensor data");  // Print error message
      Serial.println("Reason: " + fbdo.errorReason());  // Print reason for failure
    }
  }

  delay(2000); // Delay for 2 seconds before the next loop iteration
}
