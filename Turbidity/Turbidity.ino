#define BLYNK_TEMPLATE_ID "***************"
#define BLYNK_TEMPLATE_NAME "IoT Based Smart Water Quality and Level Monitoring"
#define BLYNK_AUTH_TOKEN "******************"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Pin definitions
#define TRIG_PIN D6
#define ECHO_PIN D5
#define TEMP_PIN D4
#define TURB_PIN A0

// Tank height in cm (1 foot = 30.48 cm)
#define TANK_HEIGHT 30.48

// WiFi credentials
char ssid[] = "*************";
char password[] = "******";

// Blynk timer
BlynkTimer timer;

void setup() {
  // Start serial for debugging
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT); // For open-drain simulation
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  
  // Set timer to send data every 1 second
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
  delay(1000);
}

void sendSensorData() {
  // Get water level percentage using ultrasonic sensor
  float distance = getDistance();
  float waterLevel = TANK_HEIGHT - distance;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > TANK_HEIGHT) waterLevel = TANK_HEIGHT;
  int percentage = (waterLevel / TANK_HEIGHT) * 100;
  
  // Get temperature using bit-banged 1-Wire protocol
  float temp = getTemperature();
  
  // Get turbidity
  int sensorValue = analogRead(TURB_PIN);
  float voltage = sensorValue * (3.3 / 1024.0); // ESP8266 is 3.3V
  float ntu = -1120.4 * (voltage * voltage) + 5742.3 * voltage - 4352.9;
  if (ntu < 0) ntu = 0;
  
  // Determine water quality status
  String s = (ntu < 5) ? "Good" : "Not Good";
  
  // Send data to Blynk
  Blynk.virtualWrite(V0, percentage);      // Water percentage (integer)
  Blynk.virtualWrite(V2, temp);            // Temperature (float)
  Blynk.virtualWrite(V3, ntu);             // NTU value (float)
  Blynk.virtualWrite(V1, s);               // NTU status (good or not to drink)
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Speed of sound in cm/us
  return distance;
}

// 1-Wire bit-bang functions

uint8_t oneWireReset() {
  pinMode(TEMP_PIN, OUTPUT);
  digitalWrite(TEMP_PIN, LOW);
  delayMicroseconds(480);
  pinMode(TEMP_PIN, INPUT);
  delayMicroseconds(70);
  uint8_t presence = !digitalRead(TEMP_PIN);
  delayMicroseconds(410);
  return presence;
}

void oneWireWriteBit(bool bit) {
  noInterrupts();
  if (bit) {
    pinMode(TEMP_PIN, OUTPUT);
    digitalWrite(TEMP_PIN, LOW);
    delayMicroseconds(6);
    pinMode(TEMP_PIN, INPUT);
    delayMicroseconds(64);
  } else {
    pinMode(TEMP_PIN, OUTPUT);
    digitalWrite(TEMP_PIN, LOW);
    delayMicroseconds(60);
    pinMode(TEMP_PIN, INPUT);
    delayMicroseconds(10);
  }
  interrupts();
}

bool oneWireReadBit() {
  bool bit;
  noInterrupts();
  pinMode(TEMP_PIN, OUTPUT);
  digitalWrite(TEMP_PIN, LOW);
  delayMicroseconds(6);
  pinMode(TEMP_PIN, INPUT);
  delayMicroseconds(9);
  bit = digitalRead(TEMP_PIN);
  interrupts();
  delayMicroseconds(55);
  return bit;
}

void oneWireWriteByte(uint8_t byte) {
  for (int i = 0; i < 8; i++) {
    oneWireWriteBit(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t oneWireReadByte() {
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++) {
    byte >>= 1;
    if (oneWireReadBit()) {
      byte |= 0x80;
    }
  }
  return byte;
}

float getTemperature() {
  if (!oneWireReset()) {
    return -127.0; // Error: no presence
  }
  oneWireWriteByte(0xCC); // Skip ROM
  oneWireWriteByte(0x44); // Convert T

  delay(750); // Wait for conversion (max 750ms for 12-bit)

  if (!oneWireReset()) {
    return -127.0; // Error: no presence
  }
  oneWireWriteByte(0xCC); // Skip ROM
  oneWireWriteByte(0xBE); // Read Scratchpad

  uint8_t data[9];
  for (int i = 0; i < 9; i++) {
    data[i] = oneWireReadByte();
  }

  // Calculate temperature
  int16_t raw = (data[1] << 8) | data[0];
  float temp = (float)raw / 16.0;

  return temp;
}
