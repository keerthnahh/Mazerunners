#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// ================= SERVO SETUP =================
Servo myServo1;
Servo myServo2;

// Touch pad pins
const int servo1UpPin = 2;
const int servo1DownPin = 3;
const int servo2UpPin = 4;
const int servo2DownPin = 7;

// Servo positions
int pos1 = 90;
int pos2 = 90;

// Edge detection
bool lastS1Up = HIGH;
bool lastS1Down = HIGH;
bool lastS2Up = HIGH;
bool lastS2Down = HIGH;

// ================= LED STRIP SETUP =================
#define LED_PIN 8
#define NUM_LEDS 120

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ================= TIMER SETUP =================
bool gameStarted = false;
bool gameOver = false;

unsigned long startTime = 0;
const unsigned long GAME_DURATION = 210000; // 3.5 minutes
const unsigned long LAST_10_SEC = 10000;    // 10 seconds

void setup() {
  // Attach servos
  myServo1.attach(5);
  myServo2.attach(6);

  // Initialize touch pins
  pinMode(servo1UpPin, INPUT_PULLUP);
  pinMode(servo1DownPin, INPUT_PULLUP);
  pinMode(servo2UpPin, INPUT_PULLUP);
  pinMode(servo2DownPin, INPUT_PULLUP);

  // Set initial servo positions
  myServo1.write(pos1);
  myServo2.write(pos2);

  // Initialize LED strip
  strip.begin();
  strip.show();

  // Blue = waiting for players
  setColor(0, 0, 255);
}

void loop() {

  // Read touch states
  int s1UpState = digitalRead(servo1UpPin);
  int s1DownState = digitalRead(servo1DownPin);
  int s2UpState = digitalRead(servo2UpPin);
  int s2DownState = digitalRead(servo2DownPin);

  // ================= START GAME =================
  // Start when any pad is touched for first time
  if (!gameStarted) {
    if (s1UpState == LOW || s1DownState == LOW ||
        s2UpState == LOW || s2DownState == LOW) {

      gameStarted = true;
      startTime = millis();

      // Green = game started
      setColor(0, 255, 0);
    }
  }

  // ================= TIMER =================
  if (gameStarted && !gameOver) {
    unsigned long elapsed = millis() - startTime;
    unsigned long remaining = GAME_DURATION - elapsed;

    // Orange for last 10 sec
    if (remaining <= LAST_10_SEC && remaining > 0) {
      setColor(255, 165, 0);
    }

    // Game over
    if (elapsed >= GAME_DURATION) {
      gameOver = true;

      // Red = game over
      setColor(255, 0, 0);
    }
  }

  // ================= SERVO MOVEMENT =================
  // Freeze board when game over
  if (!gameOver) {

    // -------- SERVO 1 --------
    if (s1UpState == LOW && lastS1Up == HIGH) {
      pos1 = min(pos1 + 10, 180);
      myServo1.write(pos1);
      delay(50);
    }

    if (s1DownState == LOW && lastS1Down == HIGH) {
      pos1 = max(pos1 - 10, 0);
      myServo1.write(pos1);
      delay(50);
    }

    // -------- SERVO 2 --------
    if (s2UpState == LOW && lastS2Up == HIGH) {
      pos2 = min(pos2 + 10, 180);
      myServo2.write(pos2);
      delay(50);
    }

    if (s2DownState == LOW && lastS2Down == HIGH) {
      pos2 = max(pos2 - 10, 0);
      myServo2.write(pos2);
      delay(50);
    }
  }

  // Save states
  lastS1Up = s1UpState;
  lastS1Down = s1DownState;
  lastS2Up = s2UpState;
  lastS2Down = s2DownState;

  delay(10);
}

// ================= LED FUNCTION =================
void setColor(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}
