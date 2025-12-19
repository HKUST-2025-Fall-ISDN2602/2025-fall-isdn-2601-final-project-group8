#include <Servo.h>

// -------------------------- Configuration --------------------------
// Servo Objects
Servo servoBase;    // Servo 1: Base Rotation
Servo servoShoulder;// Servo 2: Shoulder
Servo servoElbow;   // Servo 3: Elbow
Servo servoWrist;   // Servo 4: Wrist
Servo servoGripper; // Servo 5: Gripper

// Pin Assignments (ESP8266 D1 R2 PWM Pins)
const int PIN_BASE = 5;
const int PIN_SHOULDER = 4;
const int PIN_ELBOW = 12;
const int PIN_WRIST = 13;
const int PIN_GRIPPER =14;

// Initial Angles (0-180°; adjust based on your arm's mechanical zero)
int angleBase = 90;    // Base: Center position
int angleShoulder = 90;// Shoulder: Horizontal
int angleElbow = 90;   // Elbow: Bent 90°
int angleWrist = 90;   // Wrist: Horizontal
int angleGripper = 140;// Gripper: Fully open (adjust for your gripper)

// Control Parameters
const int STEP = 5;    // Angle step per key press (precision: 5°)
const int GRIP_MIN = 30;  // Gripper minimum angle (fully closed)
const int GRIP_MAX = 140; // Gripper maximum angle (fully open)

// -------------------------- Setup --------------------------
void setup() {
  // Initialize Servos
  servoBase.attach(PIN_BASE, 500, 2400);
  servoShoulder.attach(PIN_SHOULDER, 500, 2400);
  servoElbow.attach(PIN_ELBOW, 500, 2400);
  servoWrist.attach(PIN_WRIST, 500, 2400);
  servoGripper.attach(PIN_GRIPPER, 500, 2400);

  // Set Initial Positions
  updateAllServos();

  // Serial Communication (for keyboard input/feedback)
  Serial.begin(9600);
  while (!Serial) {} // Wait for Serial Monitor to open (for USB)

  // Print Welcome & Key Mapping
  Serial.println("=== 5-Servo Mechanical Arm Controller ===");
  Serial.println("Key Mapping (Each press = ±5°):");
  Serial.println("----------------------------------------");
  Serial.println("Base Rotation:  A (Left)  |  D (Right)");
  Serial.println("Shoulder:       W (Up)    |  S (Down)");
  Serial.println("Elbow:          Q (Up)    |  E (Down)");
  Serial.println("Wrist:          Z (Up)    |  C (Down)");
  Serial.println("Gripper:        X (Close) |  V (Open)");
  Serial.println("----------------------------------------");
  Serial.println("H = Show Help  |  R = Reset to Initial Positions");
  Serial.println("----------------------------------------");
  Serial.print("Initial Angles: Base="); Serial.print(angleBase);
  Serial.print(" | Shoulder="); Serial.print(angleShoulder);
  Serial.print(" | Elbow="); Serial.print(angleElbow);
  Serial.print(" | Wrist="); Serial.print(angleWrist);
  Serial.print(" | Gripper="); Serial.println(angleGripper);
}

// -------------------------- Main Loop --------------------------
void loop() {
  if (Serial.available() > 0) {
    char key = Serial.read(); // Read keyboard input
    processKeyPress(key);     // Handle key press
    updateAllServos();        // Apply new angles to servos
    printCurrentAngles();     // Feedback: Print current angles
  }
}

// -------------------------- Key Press Handler --------------------------
void processKeyPress(char key) {
  switch (key) {
    // Base Rotation (A=Left/-1°, D=Right/+1°)
    case 'A': case 'a':
      angleBase = constrain(angleBase - STEP, 0, 180);
      break;
    case 'D': case 'd':
      angleBase = constrain(angleBase + STEP, 0, 180);
      break;

    // Shoulder (W=Up/-1°, S=Down/+1°)
    case 'W': case 'w':
      angleShoulder = constrain(angleShoulder - STEP, 0, 180);
      break;
    case 'S': case 's':
      angleShoulder = constrain(angleShoulder + STEP, 0, 180);
      break;

    // Elbow (Q=Up/-1°, E=Down/+1°)
    case 'Q': case 'q':
      angleElbow = constrain(angleElbow - STEP, 0, 180);
      break;
    case 'E': case 'e':
      angleElbow = constrain(angleElbow + STEP, 0, 180);
      break;

    // Wrist (Z=Up/-1°, C=Down/+1°)
    case 'Z': case 'z':
      angleWrist = constrain(angleWrist - STEP, 0, 180);
      break;
    case 'C': case 'c':
      angleWrist = constrain(angleWrist + STEP, 0, 180);
      break;

    // Gripper (X=Close/-1°, V=Open/+1°; clamped to GRIP_MIN/MAX)
    case 'X': case 'x':
      angleGripper = constrain(angleGripper - STEP, GRIP_MIN, GRIP_MAX);
      break;
    case 'V': case 'v':
      angleGripper = constrain(angleGripper + STEP, GRIP_MIN, GRIP_MAX);
      break;

    // Reset to Initial Positions
    case 'R': case 'r':
      angleBase = 90;
      angleShoulder = 90;
      angleElbow = 90;
      angleWrist = 90;
      angleGripper = 140;
      Serial.println("=== Reset to Initial Positions ===");
      break;

    // Show Help
    case 'H': case 'h':
      printHelp();
      break;

    // Invalid Key
    default:
      Serial.print("Invalid Key: "); Serial.println(key);
      break;
  }
}

// -------------------------- Update Servos --------------------------
void updateAllServos() {
  servoBase.write(angleBase);
  servoShoulder.write(angleShoulder);
  servoElbow.write(angleElbow);
  servoWrist.write(angleWrist);
  servoGripper.write(angleGripper);
  delay(10); // Small delay for servo stability
}

// -------------------------- Feedback Functions --------------------------
void printCurrentAngles() {
  Serial.print("Current Angles: Base="); Serial.print(angleBase);
  Serial.print(" | Shoulder="); Serial.print(angleShoulder);
  Serial.print(" | Elbow="); Serial.print(angleElbow);
  Serial.print(" | Wrist="); Serial.print(angleWrist);
  Serial.print(" | Gripper="); Serial.println(angleGripper);
}

void printHelp() {
  Serial.println("\n=== Help: Key Mapping ===");
  Serial.println("Base Rotation:  A (Left)  |  D (Right)");
  Serial.println("Shoulder:       W (Up)    |  S (Down)");
  Serial.println("Elbow:          Q (Up)    |  E (Down)");
  Serial.println("Wrist:          Z (Up)    |  C (Down)");
  Serial.println("Gripper:        X (Cslose) |  V (Open)");
  Serial.println("H = Show Help  |  R = Reset Positions");
  Serial.println("----------------------------------------");
}