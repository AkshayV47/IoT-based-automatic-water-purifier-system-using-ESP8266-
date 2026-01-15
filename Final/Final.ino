#define BLYNK_TEMPLATE_ID "TMPL3LJfOLBSD"
#define BLYNK_TEMPLATE_NAME "IoT Based Smart Water Quality and Level Monitoring"
#define BLYNK_AUTH_TOKEN "qHTRvlJxC5rfIM0I3F96Fns9Dz1EfKs9"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>

// Pin definitions
#define ONE_WIRE_BUS D4
#define TURB_PIN A0
#define TRIG_PIN D6  // Trigger pin
#define ECHO_PIN D5  // Echo pin
#define MAX_SENSOR_DISTANCE 100 // Max sensor range in cm (set for safety)

// Tank height in cm (1 foot = 30.48 cm)
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_SENSOR_DISTANCE);

// Constants
const float FULL_LEVEL_DISTANCE_CM = 5; // 1 foot (full water level)
const float EMPTY_LEVEL_DISTANCE_CM = 15; // 2 feet (empty, adjust as needed)

//char ssid[] = "OPPOReno8T5G";
//char password[] = "OPPO8T5G";

char ssid[] = "Harsithbhuvan";
char password[] = "12345678";

// Blynk timer
BlynkTimer timer;

void setup() {
  // Start serial for debugging
  Serial.begin(9600);
  sensors.begin();
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TURB_PIN, INPUT); // Set A0 as input
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  
}

void loop() {
  sensors.requestTemperatures(); 
 
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Blynk.virtualWrite(V2,sensors.getTempCByIndex(0));  // Temperature (float)
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
   
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");

  float distanceCm = sonar.ping_cm();

  // Handle invalid readings (0 means out of range or error)
  if (distanceCm == 0) {
    distanceCm = EMPTY_LEVEL_DISTANCE_CM; // Treat as empty
  }

  // Calculate water level percentage
  // Percentage = ((Empty distance - Measured distance) / (Empty distance - Full distance)) * 100
  float percentage = ((EMPTY_LEVEL_DISTANCE_CM - distanceCm) / (EMPTY_LEVEL_DISTANCE_CM - FULL_LEVEL_DISTANCE_CM)) * 100;

  // Clamp percentage to 0-100%
  if (percentage > 100) percentage = 100;
  if (percentage < 0) percentage = 0;

  // Output to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm, Water Level: ");
  Serial.print(percentage);
  Blynk.virtualWrite(V0, percentage);      // Water percentage (integer)
  Serial.println("%");

  int sensorValue = analogRead(TURB_PIN);
  Serial.print("Turbidity : ");
  Serial.println(sensorValue);
  Blynk.virtualWrite(V3, sensorValue);             // NTU value (float)
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  if (turbidity < 20) {
    Blynk.virtualWrite(V1," Its CLEAR ");               // NTU Status
    Serial.print(" its CLEAR ");
  }
  if ((turbidity > 20) && (turbidity < 50)) {
    Blynk.virtualWrite(V1," Its CLOUDY ");               // NTU Status
    Serial.print(" its CLOUDY ");
  }
  if (turbidity > 50) {
    Blynk.virtualWrite(V1," Its DIRTY ");               // NTU Status
    Serial.print(" its DIRTY ");
  }
  Blynk.run();
  delay(1000);
}
