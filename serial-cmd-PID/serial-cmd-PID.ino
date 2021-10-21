/*
 * Closed-loop controller for line-following robot
 */

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rMotor = AFMS.getMotor(1);
Adafruit_DCMotor *lMotor = AFMS.getMotor(2);

// Initialize motor speed and PID variables
int avgSpeed = 40;

float kP = 0.04;
float kI = 0;
float kD = 0;
int P = 0;
int I = 0;
int D = 0;

int error = 0;
int last_error = 0;

String command = "";
bool str2hex(char *str, uint16_t *val);

/*
 * Setup function to initialize hardware and software
 */
void setup() {
  Serial.begin(115200); // Initialize serial communications at 115200 bps
           
  AFMS.begin();

  rMotor->run(FORWARD);
  lMotor->run(FORWARD);

  rMotor->setSpeed(avgSpeed);
  lMotor->setSpeed(avgSpeed);
}

/*
 * Main loop
 */
void se
void loop() {
  if (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r') { // Check if command has been entered into Serial
      parse_command();
      command = "";
    } else {
      command += ch;
    }
  }

  // If either of the outer or inner sensors are above the tape, calculate an error value proportional to the difference between the two sensors
  if (analogRead(A0) > 400 ||  analogRead(A3) > 400) {
    error = (analogRead(A0) - analogRead(A3))*2;
  }
  else {
    error = analogRead(A1) - analogRead(A2);
  }

  float C = compute_PID(error); // Scale the error value for motor control
  drive(avgSpeed+C, avgSpeed-C); // Adjust the motor speeds based on the error value
}

/*
 * Set left and right motor speeds
 */
void drive(float left, float right) {
  if (left >= 0) {
    lMotor->run(FORWARD);
    lMotor->setSpeed(left);
  }
  else {
    lMotor->run(BACKWARD);
    lMotor->setSpeed(abs(left));
  }

  if (right >= 0) {
    rMotor->run(FORWARD);
    rMotor->setSpeed(right);
  }
  else {
    rMotor->run(BACKWARD);
    rMotor->setSpeed(abs(right));
  }
}

/*
 * Scale error value for motor control
 */
float compute_PID(int error) {
  P = error;
  I += error;
  D = error - last_error;

  last_error = error;

  return kP*P + kI*I + kD*D;
}

/*
 * Parse commands sent from Serial
 */
void parse_command() {
    uint16_t val;
    if (command.startsWith("SPEED!")) {
        if (str2hex(command.substring(6), &val)) { 
          avgSpeed = val;
        }
    } else if (command.startsWith("P!")) {
        if (str2hex(command.substring(2), &val)) {
          kP = val;
        }
    } else if (command.startsWith("I!")) {
        if (str2hex(command.substring(2), &val)) {
          kI = val;
        }
    } else if (command.startsWith("D!")) {
        if (str2hex(command.substring(2), &val)) {
          kD = val;
        }
    }
}

/*
** Convert a string, str, into a 16-bit unsigned integer that is returned 
** using call by reference via val, skipping over any initial space or tab 
** characters. The function returns a boolean value that is true if the 
** conversion succeeded. The conversion is considered failed if there is 
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
