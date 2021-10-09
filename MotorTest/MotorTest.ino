#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);

uint32_t start_time;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(100);
  myMotor2->setSpeed(100);  

  start_time = millis();
}

void loop() {  
  uint32_t current_time;
  current_time = millis();

  while (current_time < start_time + 1000) {
      Serial.println("Entered loop");
      myMotor->run(FORWARD);
      myMotor2->run(FORWARD);
      current_time = millis();
  }

  myMotor->run(RELEASE);
  myMotor2->run(RELEASE);
}
