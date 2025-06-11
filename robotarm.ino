#include <Servo.h>

// Define pins
const int emgPin = A0;     // EMG sensor connected to analog pin A0 (ESP8266 has only one ADC pin)
const int servoPin = D4;   // Servo connected to digital pin D4 (GPIO2)

// Threshold values - adjust based on your EMG readings
const int emgThreshold = 400;  // EMG activation threshold
const int debounceTime = 2500; // Time between state changes (ms)

Servo myServo;
int emgValue = 0;
bool handClosed = false;
unsigned long lastActionTime = 0;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);   // Attach servo to specified pin (D4 = GPIO2)
  myServo.write(0);           // Start with hand open
}

void loop() {
  emgValue = analogRead(emgPin);   // Read EMG signal from analog pin

  Serial.print("EMG Value: ");
  Serial.println(emgValue);

  if (millis() - lastActionTime > debounceTime) {
    if (emgValue > emgThreshold && !handClosed) {
      myServo.write(180);         // Close hand
      handClosed = true;
      lastActionTime = millis();
      Serial.println("Hand closed - rotating to 180°");
    }
    else if (emgValue <= emgThreshold && handClosed) {
      myServo.write(0);           // Open hand
      handClosed = false;
      lastActionTime = millis();
      Serial.println("Hand opened - returning to 0°");
    }
  }

  delay(10);
}
