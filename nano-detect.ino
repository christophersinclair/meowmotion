#include <ArduinoBLE.h> 

const char* TARGET_MAC = "dd:34:02:09:c3:05"; // BLE beacon MAC
const int SIGNAL_PIN = 12; // PWM signal pinout on Nano 33
const unsigned long LID_OPEN_DURATION = 7000; // 7 seconds
const unsigned long SCAN_RESET_TIMEOUT = 120000; // reset scanner every 120 seconds
const int LOOP_DELAY = 200; // Delay between loops

bool lidOpen = false;
unsigned long lastDetectedTime = 0;
unsigned long lastScanResetTime = 0;

void setup() {
  if (!BLE.begin()) {
    while (1);
  }

  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);

  BLE.scanForAddress(TARGET_MAC);
}

void loop() {
  // Check if the lid has been open longer than the detection duration
  if (lidOpen && (millis() - lastDetectedTime > LID_OPEN_DURATION)) {
    sendClose();
  }

  // Reset the scanner every SCAN_RESET_TIMEOUT to ensure reliability
  if (millis() - lastScanResetTime > SCAN_RESET_TIMEOUT) {
    resetScan();
    return;
  }

  // Check for picked up beacons from the scan
  BLEDevice beacon = BLE.available();
  if (!beacon) {
    return; // next loop if no beacon found this time
  }

  lastDetectedTime = millis();
  if (!lidOpen) {
    sendOpen();
  }

  delay(200);
}

void resetScan() {
   // Reset the timer if the lid was open for the BLE scanner to restart
  if (lidOpen) {
    lastDetectedTime = millis();
  }
  
  BLE.stopScan();
  BLE.scanForAddress(TARGET_MAC);
  lastScanResetTime = millis();
}

// Send a signal to open the lid
void sendOpen() {
  digitalWrite(SIGNAL_PIN, HIGH);
  lidOpen = true;
}

// Send a signal to close the lid
void sendClose() {
  digitalWrite(SIGNAL_PIN, LOW);
  lidOpen = false;
}