#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the ESP8266 Wi-Fi library
#include <Firebase_ESP_Client.h>


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


//ultrasonic sensor
#define TRIG_PIN D1 // GPIO5
#define ECHO_PIN D2 // GPIO4


// Insert your network credentials
#define WIFI_SSID "Mavericks"
#define WIFI_PASSWORD "$weetC0rn"


// Insert Firebase project API Key
#define API_KEY "AIzaSyCjCT6oFOmZuYjbyy5X-T3kHmpi-DjFBX0"


// Insert Firebase Database URL (without https://)
#define DATABASE_URL "https://hydroponic-bd230-default-rtdb.asia-southeast1.firebasedatabase.app/"


// Define Firebase Data object
FirebaseData fbdo;


FirebaseAuth auth;
FirebaseConfig config;


unsigned long sendDataPrevMillis = 0;
bool signupOK = false;          // Since we are doing an anonymous sign-in


void setup(){
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
 
  // Sign up anonymously to Firebase
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("Firebase SignUp Failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;  // For token generation status
 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop(){
  long duration, distance;

  // Check if Firebase is ready and data can be sent
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Send a 10us pulse to trigger pin
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read the echo pin duration
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate distance in centimeters
    distance = (duration / 2) / 29.1;

    // Check if any reads failed
    if (isnan(distance)) {
      Serial.println("Failed to read from Ultrasonic sensor!");
      return;
    }

    // Send Temperature data to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "Ultrasonic/distance", distance)){
      Serial.println("Ultrasonic sensor data sent to Firebase successfully");
    } else {
      Serial.println("Failed to send sensor data");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }

  delay(2000); // Delay for half a second

}
