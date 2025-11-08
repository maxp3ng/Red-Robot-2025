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


//auton globals
bool autonGo = false;

float pos, lastPos, error, lastError;
float p,i,d;

//tweak implement gains!
float Kp = 0.5;
float Ki = 0;
float Kd = 0;
float speed = 0.5;

void autonDrive(float angle){
  drive(speed+angle, speed-angle);
}

void auton()
{
  if (btnB) {autonGo = true;}
  if (!autonGo) {return;}
  int nSense = 4;
  float nSenseAv = (float)(nSense-1)/2.0;
  int sensors[nSense];
  float mean;
  float sum = 0.0;
  
  Serial.print("Line sensors =");
  RR_getLineSensors(sensors);
  for (int i = 0; i < nSense; ++i)
  {
    // TODO tweak offset based weighting 
    sum += (sensors[i]*(i-nSenseAv));

    Serial.print(sensors[i]);
    Serial.print(" ");
  }
  mean = sum / nSense;

  pos = mean;
  error = pos-nSenseAv;

  // we love PID!
  p = error;

  if (i+error>100) {i=100;}
  else if (i+error<-100) {i=-100;}
  else {i += error;} 
  d = error - lastError;
  
  lastPos = pos;
  lastError = error;
  int pid = (Kp*p + Ki*i + Kd*d);
  float maxPID = 5000;
  float turn = pid/maxPID;
  turn = constrain(turn, -1.0, 1.0);
  autonDrive(turn);


  Serial.println();

  Serial.print("p: "); Serial.println(p);
  Serial.print("i: "); Serial.println(i);
  Serial.print("d: "); Serial.println(d);
  Serial.print("mean : "); Serial.println(mean);
  Serial.print("sum: "); Serial.println(sum);
  Serial.print("Pos: "); Serial.println(pos);
  Serial.print(" Error: "); Serial.println(error);
  Serial.print(" PID: "); Serial.println(pid);
  Serial.print(" turn: "); Serial.println(turn);
  
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
  //Rotate right
  Serial.println(RR_getUltrasonic());
  if (RR_getUltrasonic()==0||RR_getUltrasonic()>2.5){
    Serial.println('e');
    if(dir==true){
      rightX=0.0;
      leftY=-1.0;
    } else {
    //Rotate left
      leftY=1.0;
      rightX=0.0;
    }
  }
}

void drive(float l, float r) {
  Serial.println();

  Serial.print("l: "); Serial.println(l);
  Serial.print("r: "); Serial.println(-1.0*r);

  RR_setMotor1(l);
  RR_setMotor2(-1.0*r);
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
  //auton button is B
  Serial.print(btnB ? 1 : 0);
  Serial.print(btnX ? 1 : 0);
  Serial.print(btnY ? 1 : 0);
  Serial.println();
}

void loop()
{
  teleopRead();
  // read the ultrasonic sensors
  //if (btnRB) {
  //  alignToBall(true);
  //} else if (btnLB) {
    //alignToBall(false);
    //drive(leftY+rightX,leftY-rightX);

  //int sensors[6];
  //printUltrasonic();
  // Serial.print("Line sensors=");
  // RR_getLineSensors(sensors);
  // for (int i = 0; i < 6; ++i)
  // {
  //   Serial.print(sensors[i]);
  //   Serial.print(" ");
  // }
  //int angle = 1;
  //drive(0-angle, angle);

  auton();

  // This is important - it sleeps for delayTime/100 seconds
  // Running the code too fast will overwhelm the microcontroller and peripherals
  delay(delayTime);
  autonTime += delayTime;
}

// vim: tabstop=2 shiftwidth=2 expandtab
