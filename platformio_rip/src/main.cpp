#include <Arduino.h>

// Include the generated controller
extern "C" {
    #include "controller.h"
}


// PIN DEFINITIONS

// Motor Encoder Pins
const int ENCODER_PIN_A = 2;
const int ENCODER_PIN_B = 3;
const int COUNTS_PER_REV = 1080;

// YT3806 Pendulum Encoder Pins  
const int YT3806_ENCODER_A_PIN = 7;  
const int YT3806_ENCODER_B_PIN = 10; 
const int YT3806_COUNTS_PER_REVOLUTION = 2400;

// BTS motor driver pins
const int BTS_RPWM_PIN = 5;
const int BTS_LPWM_PIN = 6;
const int BTS_REN_PIN = 9;
const int BTS_LEN_PIN = 8;


// ENCODER VARIABLES - HYBRID APPROACH
volatile double phi = 0.0f;        
volatile double theta = 0.0f;      

// Motor encoder state - FAST POLLING
volatile long encoderCount = 0;
volatile int motorLastEncoded = 0;

// YT3806 encoder state - YOUR PROVEN INTERRUPT METHOD
volatile long yt3806EncoderCount = 0;
volatile int lastYT3806Encoded = 0;

unsigned long lastPrintTime = 0;
unsigned long lastEncoderCheck = 0;
int debugCounter = 0;


// YT3806 INTERRUPT HANDLER - YOUR PROVEN WORKING METHOD
void updateYT3806Encoder() {
    int MSB = digitalRead(YT3806_ENCODER_A_PIN);
    int LSB = digitalRead(YT3806_ENCODER_B_PIN);
    
    int encoded = (MSB << 1) | LSB;
    int sum = (lastYT3806Encoded << 2) | encoded;
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        yt3806EncoderCount++;
    }
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        yt3806EncoderCount--;
    }
    
    lastYT3806Encoded = encoded;
}

// MOTOR ENCODER READING - OPTIMIZED POLLING
void readMotorEncoder() {
    int MSB = digitalRead(ENCODER_PIN_A);
    int LSB = digitalRead(ENCODER_PIN_B);
    
    int encoded = (MSB << 1) | LSB;
    int sum = (motorLastEncoded << 2) | encoded;
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        encoderCount++;
    }
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        encoderCount--;
    }
    
    motorLastEncoded = encoded;
}

// ENCODER READING FUNCTIONS
float readArmAngle() {
    float angle_rad = (float)encoderCount * 2.0f * PI / COUNTS_PER_REV;
    while (angle_rad > PI) angle_rad -= 2.0f * PI;
    while (angle_rad < -PI) angle_rad += 2.0f * PI;
    return angle_rad;
}

float getArmAngleDegrees() {
    float angle = (float)encoderCount * 360.0f / COUNTS_PER_REV;
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}

float readPendulumAngle() {
    float angle_rad = (yt3806EncoderCount * 2.0f * PI) / YT3806_COUNTS_PER_REVOLUTION;
    while (angle_rad < 0) angle_rad += 2.0f * PI;
    while (angle_rad >= 2.0f * PI) angle_rad -= 2.0f * PI;
    
    if (angle_rad > PI) {
        angle_rad -= 2.0f * PI;
    }
    return angle_rad;
}


// CONTROLLER INTERFACE
float computeControlInput() {
    rtU.phi = phi;
    rtU.theta = theta;
    controller_step();
    return rtY.u;
}

void setMotorTorque(float torque) {
    int pwmValue = (int)(abs(torque) * 255.0f / 0.635f);
    pwmValue = constrain(pwmValue, 0, 255);
    
    if (torque > -0.001f) {
        analogWrite(BTS_RPWM_PIN, pwmValue);
        analogWrite(BTS_LPWM_PIN, 0);
    } else if (torque < 0.001f) {
        analogWrite(BTS_RPWM_PIN, 0);
        analogWrite(BTS_LPWM_PIN, pwmValue);
    } else {
        analogWrite(BTS_RPWM_PIN, 0);
        analogWrite(BTS_LPWM_PIN, 0);
    }
}

float radToDeg(float rad) {
    return rad * 180.0f / PI;
}


// SETUP
void setup() {
    Serial.begin(115200);
    delay(3000);
    
    Serial.println("HYBRID SOLUTION!");
    Serial.println("YT3806: Your proven interrupt method");
    Serial.println("Motor: Optimized fast polling");
  
    
    // Configure motor driver pins
    pinMode(BTS_RPWM_PIN, OUTPUT);
    pinMode(BTS_LPWM_PIN, OUTPUT);
    pinMode(BTS_REN_PIN, OUTPUT);
    pinMode(BTS_LEN_PIN, OUTPUT);
    digitalWrite(BTS_REN_PIN, HIGH);
    digitalWrite(BTS_LEN_PIN, HIGH);
    
    // Setup encoder pins
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
    pinMode(YT3806_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(YT3806_ENCODER_B_PIN, INPUT_PULLUP);
    
    // Initialize encoder states
    motorLastEncoded = (digitalRead(ENCODER_PIN_A) << 1) | digitalRead(ENCODER_PIN_B);
    lastYT3806Encoded = (digitalRead(YT3806_ENCODER_A_PIN) << 1) | digitalRead(YT3806_ENCODER_B_PIN);
    
    // Setup YT3806 interrupts - YOUR PROVEN METHOD
    attachInterrupt(digitalPinToInterrupt(YT3806_ENCODER_A_PIN), updateYT3806Encoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(YT3806_ENCODER_B_PIN), updateYT3806Encoder, CHANGE);
    
    // Initialize controller
    controller_initialize();
    
    lastPrintTime = millis();
    lastEncoderCheck = micros();
    
    Serial.println("YT3806: Interrupt-based (no missed pulses)");
    Serial.println("Motor: Fast polling (acceptable for phi)");
    Serial.println("Format: θ(°) φ(°) τ(Nm) M_cnt Y_cnt");
    Serial.flush();
}


// MAIN LOOP - FAST POLLING
void loop() {
    // Read motor encoder as fast as possible (polling - may miss some pulses at high speed)
    if (micros() - lastEncoderCheck >= 50) {  // Even faster - every 50 microseconds
        lastEncoderCheck = micros();
        readMotorEncoder();
        debugCounter++;
    }
    
    // Control loop every 1ms
    static unsigned long lastControlTime = 0;
    if (millis() - lastControlTime >= 1) {
        lastControlTime = millis();
        
        theta = readPendulumAngle();
        phi = readArmAngle();
        
        float tauPhi = computeControlInput();
        setMotorTorque(tauPhi);
    }
    
    // Print debug every 200ms
    if (millis() - lastPrintTime >= 200) {
        lastPrintTime = millis();
        
        long motorRaw = encoderCount;
        long yt3806Raw = yt3806EncoderCount;
        float currentTorque = computeControlInput();
        
        Serial.print("θ:");
        Serial.print(radToDeg(theta), 2);
        Serial.print("° φ:");
        Serial.print(getArmAngleDegrees(), 2);
        Serial.print("° τ:");
        Serial.print(currentTorque, 4);
        Serial.print("Nm M:");
        Serial.print(motorRaw);
        Serial.print(" Y:");
        Serial.print(yt3806Raw);
        Serial.print(" C:");
        Serial.println(debugCounter);
        
        Serial.flush();
        debugCounter = 0;
    }
    
    // Handle serial reset
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'r' || cmd == 'R') {
            encoderCount = 0;
            yt3806EncoderCount = 0;
            Serial.println("Both encoders reset");
        }
    }
}