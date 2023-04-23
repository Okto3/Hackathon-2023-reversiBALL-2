const int dirPin = 8;
const int stepPin = 7;

const byte limit1 = 9;
const byte limit2 = 10;

const int trigPin = 5; 
const int echoPin = 6;

float Kp = 1.2;
float Kd = 0.01;

float KpKick = 0.5;

int stepperPos = 400;
int amountToMove = 0;
float amountToKick = 0;

int leftLED = 2;
int rightLED = 3;

float t;
float dt;
float tPrev = 0;
float error;
float errorPrev = 0;

int minPos = 0;
int maxPos = 0;
int middlePosition = 0;

int steps = 0;
 


void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(limit1, INPUT);
  pinMode(limit2, INPUT);

  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  //perform a calibration to know that it starts in the centre
  delay(1000);
  Serial.println("Calibrating:");
  calibrate();
  Serial.println("Calibration Complete");
  delay(1000);
}


void calibrate() {
  digitalWrite(dirPin, LOW);
  delay(1);
  //moves until a limit switch is pressed
  while(digitalRead(9) != 1){
    digitalWrite(stepPin, HIGH);
    delay(1);
    digitalWrite(stepPin, LOW);
    delay(1);
  }
  // sets the minimum pos
  minPos = analogRead(A0);
  Serial.println("finished first cal");
  digitalWrite(dirPin, HIGH);
  delay(1);
  //moves to the other end limit;
  while(digitalRead(10) != 0){
    digitalWrite(stepPin, HIGH);
    delay(1);
    digitalWrite(stepPin, LOW);
    delay(1);
  }
  maxPos = analogRead(A0);
  // calculates the average between the max and min
  middlePosition = (maxPos + minPos)/2;
  Serial.println("midle pos:");
  Serial.println(middlePosition);
  Serial.println(minPos);
  Serial.println(maxPos);
  digitalWrite(dirPin, LOW);
  // moves halfway in between
  stepForward(1500, 800);
  delay(2000);

  steps = 0;
}


void loop() {
  // checks if the left limit switch is pressed, on only releases if the angle is such that it will correct itself
  while(digitalRead(9) == 1 and analogRead(A0) > middlePosition){
    Serial.println("left is pressed");
    // turns on an led indicating that user intevention is required
    digitalWrite(leftLED, HIGH);
    delay(5);
  }
  // checks the other limit switch
  while(digitalRead(10) == 0 and analogRead(A0) < middlePosition){
    Serial.println("right is pressed");
    // turns on the other led
    digitalWrite(rightLED, HIGH);
    delay(5);
  }
  // turns the LEDs off
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);

  // read the position of the potentiometer
  int pot = analogRead(A0);
  
  // calculates the error between the middle position and where it is
  int error = pot - middlePosition
  //Serial.println(steps);
  if(abs(error) < 5) {
    //in stable equilibrium, amountToKick is how much to move it to get it out of an equilibrium and move it in the correct direction
    amountToKick = KpKick * steps * -1; 
  }
  else{
    amountToKick = 0;
  }
  // this is the basic control loop thats just a proportional controller + the kick which it really doesn't like
  amountToMove = error * Kp + amountToKick; // + (Kd * (error - errorPrev) / dt);

  //keep track of how far it's moved
  steps = steps + amountToMove;

  //this code actually moves it
  if(amountToMove > 0){
    // sets the direction and amount to move
    digitalWrite(dirPin, 0);
    stepForward(amountToMove, 600);
  }
  else{
    //moves it the other way, making the amount to move positive
    digitalWrite(dirPin, 1);
    stepForward(-1*amountToMove, 600);
  }  
}

void stepForward (int steps, int delayTime) {
  // this function moves the steppers
  for(int i=0; i<steps; i++){
    
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
    if(amountToMove > 0){
      // tracks how far it's gone
      amountToMove--;
    }
    else{
      amountToMove++;
    }
  }
}
