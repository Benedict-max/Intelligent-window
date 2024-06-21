#include <Servo.h>

// Define sensor pins
#define LDR_PIN A0
#define RAIN_PIN A1
#define TEMP_PIN A2

// Define servo and relay pins
#define WINDOW_SERVO_PIN 9
#define RELAY_PIN 10

// Threshold values
int lightThreshold = 300;   // Adjust based on your light sensor's readings
int rainThreshold = 500;    // Adjust based on rain sensor analog values
int temperatureThreshold = 30;  // Temperature threshold in Celsius for hot conditions

Servo windowServo;

void setup() {
  Serial.begin(9600);

  // Initialize sensor pins
  pinMode(RAIN_PIN, INPUT);
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is off initially
  
  // Attach servo to pin
  windowServo.attach(WINDOW_SERVO_PIN);
  windowServo.write(0); // Initial position, window closed

  // Print initial status to Serial Monitor
  Serial.println("Automated Window and Clothline System Initialized");
}

void loop() {
  // Read sensor values
  int lightValue = analogRead(LDR_PIN);
  int rainValue = digitalRead(RAIN_PIN);
  int tempValue = analogRead(TEMP_PIN);
  
  // Convert tempValue to Celsius
  float temperature = (tempValue / 1024.0) * 500.0;

  // Print sensor readings to Serial Monitor
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print(" | Rain: ");
  Serial.print(rainValue ? "Yes" : "No");
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Automated Window Control
  if (lightValue >= lightThreshold && temperature >= temperatureThreshold) {
    // Open the window if it's light enough and hot inside
    if (windowServo.read() != 90) {
      windowServo.write(90);  // Adjust as needed to open the window
      Serial.println("Window opened due to sufficient light and high temperature");
    }
  } else {
    // Close the window if it's dark, raining, or not hot enough
    if (windowServo.read() != 0) {
      windowServo.write(0); // Adjust as needed to close the window
      Serial.println("Window closed due to darkness, rain, or low temperature");
    }
  }

  // Automated Clothline Control
  if (lightValue >= lightThreshold && rainValue == LOW) {
    // Extend clothline if it's light and not raining
    digitalWrite(RELAY_PIN, HIGH); // Activate relay to power the motor
    Serial.println("Clothline extended due to sufficient light and no rain");
  } else {
    // Retract clothline if it starts raining or gets dark
    digitalWrite(RELAY_PIN, LOW); // Deactivate relay to stop the motor
    Serial.println("Clothline retracted due to rain or darkness");
  }

  delay(1000); // Wait 1 second before next loop
}
