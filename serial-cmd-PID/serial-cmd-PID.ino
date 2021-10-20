#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rMotor = AFMS.getMotor(1);
Adafruit_DCMotor *lMotor = AFMS.getMotor(2);

// Set initial motor speeds
int avgSpeed = 40;

float kP = 0.04;
float kI = 0;
float kD = 0;

int P = 0;
int I = 0;
int D = 0;

int last_error = 0;

String command = "";
bool str2hex(char *str, uint16_t *val);


void setup() {
  // Initialize serial communications at 9600 bps:
  Serial.begin(115200);  
           
  AFMS.begin();

  rMotor->run(FORWARD);
  lMotor->run(FORWARD);

  rMotor->setSpeed(avgSpeed);
  lMotor->setSpeed(avgSpeed);
}


void loop() {
  if (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r') {
      parse_command();
      command = "";
    } else {
      command += ch;
    }
  }

  int error = 0;
  if (analogRead(A0) > 400 ||  analogRead(A3) > 400) {
    error = (analogRead(A0) - analogRead(A3))*2;
  }
  else {
    error = analogRead(A1) - analogRead(A2);
  }

  float C = compute_PID(error);
  drive(avgSpeed+C, avgSpeed-C);

  Serial.print(avgSpeed+C); Serial.print(',');
  Serial.print(avgSpeed-C); Serial.print(',');
  Serial.print(analogRead(A0)); Serial.print(',');
  Serial.print(analogRead(A1)); Serial.print(',');
  Serial.print(analogRead(A2)); Serial.print(',');
  Serial.print(analogRead(A3)); Serial.print(',');
  Serial.println(error);
}


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


float compute_PID(int error) {
  P = error;
  I += error;
  D = error - last_error;

  last_error = error;

  return kP*P + kI*I + kD*D;
}


void parse_command() {
  uint16_t val;
  if (command.startsWith("SPEED!")) {
    avgSpeed = command.substring(6).toInt();
  } else if (command.startsWith("P!")) {
    kP = command.substring(2).toFloat();
  } else if (command.startsWith("I!")) {
    kI = command.substring(2).toFloat();
  } else if (command.startsWith("D!")) {
    kI = command.substring(2).toFloat();
  }
}
