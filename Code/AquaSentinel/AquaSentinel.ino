#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Pin assignments
int trigPin = 10;       // Trigger pin for ultrasonic sensor
int echoPin = 8;        // Echo pin for ultrasonic sensor
const int hook = 2;     // Hook control pin

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Alarm pin
int alarm = 6;  

// Relay control pins
int relayStoragePin = 13;
int relayAgriculturePin = 4;

// Analog sensor pins
int waterStorageSensor = A0;
int agricultureSensor = A1;
Servo bridgeServoRight;
Servo bridgeServoLeft;
Servo dampServo;
Servo agricultureServo;

// Ultrasonic sensor variables
float duration_us, distance_cm;

// Flag to ensure alarm only triggers once
int alarmOnce = 2;
int bridge = 2;


void setup() {
  Serial.begin(9600);

  // Attach servo motors
  dampServo.attach(9);
  agricultureServo.attach(12);
  bridgeServoRight.attach(3);
  bridgeServoLeft.attach(11);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Pin mode configurations
  pinMode(echoPin, INPUT);
  pinMode(waterStorageSensor, INPUT);
  pinMode(agricultureSensor, INPUT);
  pinMode(hook, OUTPUT);

  // Alarm setup
  pinMode(alarm, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(relayAgriculturePin, OUTPUT);
  pinMode(relayStoragePin, OUTPUT);

  // Initial states
  digitalWrite(alarm, HIGH);
  digitalWrite(trigPin, HIGH);
  digitalWrite(hook, HIGH);
  
 // Initial LCD message
  lcd.setCursor(0, 0);
  lcd.print("Dam: Close" );

  // Initial servo positions
  dampServo.write(0);
  bridgeServoLeft.write(20);
  bridgeServoRight.write(0);
}

void loop() {
  // Ultrasonic sensor reading
  digitalWrite(alarm, LOW);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //calculation
  duration_us = pulseIn(echoPin, HIGH);
  distance_cm = 0.017 * duration_us;
  
  Serial.print(distance_cm);
  Serial.println("cm");

  // Control hook based on distance
  if (distance_cm <= 12){
    digitalWrite(hook, HIGH);
    delay(4000);
  } else {
    digitalWrite(hook, LOW);
    delay(4000);
  }

  // Call additional features based on distance and sensor readings
  features(distance_cm, waterStorageSensor);
}

// Function to handle features
void features(int distance_cm, int waterStorageSensor){
  if (distance_cm <= 28){
    damLevel(distance_cm);
  } else if (distance_cm == 0 or distance_cm > 20) {
    Serial.println("ALERT OVERFLOW");
  }

  agriculturePath();

  if (waterStorageSensor < 400) {
    agriculture();
  }
}

// Function to control dam level based on distance
void damLevel(int distance_cm) {
  int currentLevel;
  int alertLevel = 8;

  // Calculate dam water level
  currentLevel = 14 - (distance_cm / 2);
  Serial.print("Level: ");
  Serial.println(currentLevel);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dam: Closed ");

  // Initialize alarm flag
  if (alarmOnce == 2){
    alarmOnce = 1;
  }

  // Check if dam level is above alert level
  if (currentLevel >= alertLevel){
    // Trigger alarm and open the dam
    if (alarmOnce == 1){
        for (int z = 0; z <= 10; z++) {
        lcd.setCursor(0, 0);
        lcd.print("Timer:" );
        lcd.print(z);
        lcd.print("      ");
        delay(800); 
      }
      for (int i = 1; i <= 12; i++) {
        digitalWrite(alarm, LOW);
        delay(150);
        digitalWrite(alarm, HIGH);
        delay(150);
      }

      for (int angle = 0; angle <= 40; angle++) {
        bridgeServoLeft.write(angle + 20);
        bridgeServoRight.write(angle);
        delay(50);
      }

      delay(2000);
      alarmOnce = 0;
      bridge = 1;
    }
    dampServo.write(180);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dam: Open");

  }  else if (currentLevel < alertLevel){
    // Close the dam and reset the alarm
    dampServo.write(0);
    if (bridge == 1){
      for (int angle = 40; angle >= 0; angle--) {
        bridgeServoLeft.write(angle + 20);
        bridgeServoRight.write(angle);
        delay(50);
      }
      bridge = 0;
    }

    alarmOnce = 1;
  }
}

// Function to control water storage based on sensor readings
void agriculturePath() {
  int dryThreshold = 500;
  int water = analogRead(waterStorageSensor);

  Serial.print("Water: ");
  Serial.println(water);

  // Check if water level is below the threshold
  if (water > dryThreshold) {
    Serial.println("No Water!");
    digitalWrite(relayStoragePin, HIGH);
  } else {
    digitalWrite(relayStoragePin, LOW);
    Serial.println("LIMITED");
  }

  delay(1000);
}

// Function to control agriculture based on soil moisture
void agriculture() {
  int soilValue = analogRead(agricultureSensor);

  Serial.print("Soil Moisture Level: ");
  Serial.println(soilValue);

  // Check if soil moisture is below a certain threshold
  if (soilValue > 500) {
    Serial.println("No Water!");
    digitalWrite(relayAgriculturePin, HIGH);
  } else {
    digitalWrite(relayAgriculturePin, LOW);
    Serial.println("LIMITED");
  }
  delay(1000);
}
