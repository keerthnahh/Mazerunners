// ===== Pin Definitions =====
const int irSensorPin = 2;      // IR sensor digital output
const int vibrationPin = 9;     // Vibration motor (through transistor/MOSFET)
const int ledPin = 13;          // Optional LED for visual debugging

// ===== Variables =====
bool hazardDetected = false;

void setup() {
  pinMode(irSensorPin, INPUT);
  pinMode(vibrationPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(vibrationPin, LOW);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(irSensorPin);

  // Many IR modules output LOW when object detected
  if (sensorValue == LOW) {
    hazardDetected = true;
  } else {
    hazardDetected = false;
  }

  if (hazardDetected) {
    Serial.println("Hazard Detected!");

    // Turn on vibration motor
    digitalWrite(vibrationPin, HIGH);
    digitalWrite(ledPin, HIGH);

    delay(500); // vibration duration
  } else {
    digitalWrite(vibrationPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(50);
}
