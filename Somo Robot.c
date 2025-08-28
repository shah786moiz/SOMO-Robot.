#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSerial(10, 11); // Bluetooth
Servo myServo;

// Motor driver pins
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;

// Ultrasonic sensor pins
const int trigPin = A0;
const int echoPin = A1;

void setup() {
    Serial.begin(9600);
    BTSerial.begin(9600);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    myServo.attach(9);
    myServo.write(90); // Center position

    Serial.println("Bluetooth Car with Obstacle Avoidance Ready!");
}

void loop() {
    obstacleAvoidance();

    if (BTSerial.available()) {
        char command = BTSerial.read();
        Serial.print("Received Command: ");
        Serial.println(command);

        executeCommand(command);
    }
}

void obstacleAvoidance() {
    long duration;
    int distance;
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; // Convert to cm

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 20) { // Obstacle detected within 20cm
        stopCar();
        myServo.write(0); // Rotate servo left
        delay(500);
        myServo.write(180); // Rotate servo right
        delay(500);
        myServo.write(90); // Reset position
        backwardCar();
        delay(700);
        turnRight();
        delay(500);
        stopCar();
    }
}

void executeCommand(char command) {
    switch (command) {
        case 'U': forwardCar(); break;
        case 'D': backwardCar(); break;
        case 'L': turnLeft(); break;
        case 'R': turnRight(); break;
        case 'S': stopCar(); break;
    }
}

void forwardCar() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void backwardCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stopCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
