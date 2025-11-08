// Replace 12345 with the correct team number and then uncomment the line below.
#define TEAM_NUMBER 26

#ifndef TEAM_NUMBER
#error "Define your team number with `#define TEAM_NUMBER 12345` at the top of the file."
#elif TEAM_NUMBER < 1 || 40 < TEAM_NUMBER
#error "Team number must be within 1 and 40"
#endif

void setup()
{
  Serial.begin(115200);
}

int temp = 0;
int autonTime = 0;
int delayTime = 20;

float rightX,rightY,leftX,leftY;
bool btnA,btnB,btnX,btnY,btnRB,btnLB;

void auton()
{
  int sensors[6];

  RR_getLineSensors(sensors);

}

void teleopRead() {
  rightX = RR_axisRX();
  rightY = RR_axisRY();
  leftX = RR_axisLX();
  leftY = RR_axisLY();
  btnA = RR_buttonA();
  btnB = RR_buttonB();
  btnX = RR_buttonX();
  btnY = RR_buttonY();
  btnRB = RR_buttonRB();
  btnLB = RR_buttonLB();
}

void alignToBall(bool dir){
  //Rotate left
  if(dir==true){
    
  } else {
  //Rotate right
  
  }
}

void drive(int l, int r) {
  RR_setMotor1(l);
  RR_setMotor2(r);
}



void controlServo1(int l, int r){
      // we can't move a servo less than 0 degrees

  if (temp > 0)
      temp -= l;

      // we can't move a servo past 180 degrees
  // for continuous rotation, try using a DC motor
  if (temp < 180)
      temp += r;
}

void printUltrasonic() {
  Serial.print("Ultrasonic=");
  Serial.print(RR_getUltrasonic());
  Serial.print(" ;; ");
}

void printButtons() {
  Serial.print(btnA ? 1 : 0);
  Serial.print(btnB ? 1 : 0);
  Serial.print(btnX ? 1 : 0);
  Serial.print(btnY ? 1 : 0);
  Serial.println();
}

void loop()
{
  teleopRead();
  // read the ultrasonic sensors
  drive(leftY+rightX,leftY-rightX);

  if (btnRB) {
    alignToBall(true)
  } else if (btnLB) {
    alignToBall(false)
  }

  int sensors[6];

  Serial.print("Line sensors=");
  RR_getLineSensors(sensors);
  for (int i = 0; i < 6; ++i)
  {
    Serial.print(sensors[i]);
    Serial.print(" ");
  }

  auton();
  // This is important - it sleeps for delayTime/100 seconds
  // Running the code too fast will overwhelm the microcontroller and peripherals
  delay(delayTime);
  autonTime += delayTime;
}

// vim: tabstop=2 shiftwidth=2 expandtab
