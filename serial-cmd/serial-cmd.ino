#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rMotor = AFMS.getMotor(1);
Adafruit_DCMotor *lMotor = AFMS.getMotor(2);

// Set initial motor speeds
int rSpeed = 50;
int lSpeed = 50;

// Analog input pins that the distance sensors are attached to
const int rAnalogPin = A0;  
const int lAnalogPin = A1;  

String command = "";
bool str2hex(char *str, uint16_t *val);


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
  rMotor->setSpeed(rSpeed);
  lMotor->setSpeed(lSpeed);  
}


void loop() {  
  rMotor->run(FORWARD);
  lMotor->run(FORWARD);
      
  rMotor->setSpeed(rSpeed);
  lMotor->setSpeed(lSpeed);  

  if (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r') {
      parse_command();
      command = "";
    } else {
      command += ch;
    }
  }
}


void parse_command() {
  uint16_t val;

  if (command.equals("RSENSOR?")) {
    Serial.print(analogRead(rAnalogPin));
    Serial.print("\r\n");
  } else if (command.equals("LSENSOR?")) {
    Serial.print(analogRead(lAnalogPin));
    Serial.print("\r\n");
  } else if (command.startsWith("RMOTOR!")) {
    if (str2hex(command.substring(7), &val)) {
      rSpeed = val;
    }
  } else if (command.startsWith("LMOTOR!")) {
    if (str2hex(command.substring(7), &val)) {
      lSpeed = val;
    }
  } 
}


/*
** Converts a string, str, into a 16-bit unsigned integer that is returned 
** using call by reference via val, skipping over any initial space or tab 
** characters.  The function returns a boolean value that is true if the 
** conversion succeeded.  The conversion is considered failed if there is 
** no valid sequence of hex digits or if the sequence of hex digits does 
** not end with the end of the string.
*/
bool str2hex(String str, uint16_t *val) {
  uint8_t pos = 0;

  if (str.length() == 0)
    return false;

  while ((str.charAt(pos) == ' ') || (str.charAt(pos) == '\t')) {
    pos++;
  }

  *val = 0;
  while (pos < str.length()) {
    if ((str.charAt(pos) >= '0') && (str.charAt(pos) <= '9')) {
      *val = (*val << 4) + (str.charAt(pos) - '0');
    } else if ((str.charAt(pos) >= 'a') && (str.charAt(pos) <= 'f')) {
      *val = (*val << 4) + 10 + (str.charAt(pos) - 'a');
    } else if ((str.charAt(pos) >= 'A') && (str.charAt(pos) <= 'F')) {
      *val = (*val << 4) + 10 + (str.charAt(pos) - 'A');
    } else {
      return false;
    }
    pos++;
  }

  return true;
}
