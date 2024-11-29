#include <Servo.h>

const int SERVO_PIN = 9;  // Servo connected to PWM pin
const int SIGNAL_PIN = 2; // Pin receiving signal from Nano
const int CLOSE_LED_PIN = 10; // Pin lighting red LED
const int OPEN_LED_PIN = 11; // Pin lighting green LED
Servo servo;

bool lidOpen = false;

void setup() {
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(CLOSE_LED_PIN, OUTPUT);
  pinMode(OPEN_LED_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(0); // Initialize with lid closed

}

void loop() {
  if (digitalRead(SIGNAL_PIN) == HIGH) {
    if (!lidOpen) {
      openLid();
    }
  } else {
    if (lidOpen) {
      closeLid();
    }
  }

  delay(1000);
}

// Slowly open lid to not smack the cat in the face
void openLid() {
  digitalWrite(CLOSE_LED_PIN, LOW);
  digitalWrite(OPEN_LED_PIN, HIGH);
  for (int pos = 0; pos <= 90; pos += 1) {
    servo.write(pos);
    delay(50);
  }

  lidOpen = true;
}

// Slowly close lid to not crush the cat's head
void closeLid() {
  digitalWrite(OPEN_LED_PIN, LOW);
  digitalWrite(CLOSE_LED_PIN, HIGH);
  for (int pos = 90; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(50);
  }

  lidOpen = false;
}