#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rMotor = AFMS.getMotor(1);
Adafruit_DCMotor *lMotor = AFMS.getMotor(2);

// Analog input pins that the distance sensors are attached to
const int rAnalogPin = A0;  
const int lAnalogPin = A1; 

// Value reads from distance sensors
int rSensorValue = 0;        
int lSensorValue = 0;       

uint32_t start_time;

const int highSpeed = 50;
const int lowSpeed = 25;


void setup() {
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);           

  // Check if the motor shield has been found
  if (!AFMS.begin()) {        
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  rMotor->setSpeed(highSpeed);
  lMotor->setSpeed(highSpeed);  

  start_time = millis();
}


void loop() {  
  uint32_t current_time;
  current_time = millis();

  while (current_time < start_time + 1000000) {
    
      Serial.println("Entered loop");
      
      rMotor->run(FORWARD);
      lMotor->run(FORWARD);
      current_time = millis();

      rSensorValue = analogRead(rAnalogPin);
      lSensorValue = analogRead(lAnalogPin);

      // Decrease right motor if right sensor goes on tape
      if (rSensorValue > 995) {
        rMotor->setSpeed(lowSpeed);
      } 
      else if (rSensorValue <= 995) {
        rMotor->setSpeed(highSpeed);
      }

      // Decrease left motor if left sensor goes on tape
      if (lSensorValue > 995) {
        lMotor->setSpeed(lowSpeed);
      } 
      else if (lSensorValue <= 995) {
        lMotor->setSpeed(highSpeed);
      }
      
  }

  rMotor->run(RELEASE);
  lMotor->run(RELEASE);
}
