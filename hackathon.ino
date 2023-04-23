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

  pinMode(trigPin, OUTPUT); // set the trigger pin as an output
  pinMode(echoPin, INPUT);


  delay(1000);
  Serial.println("Calibrating:");
  calibrate();
  Serial.println("Calibration Complete");
  delay(1000);
}


void calibrate() {
  digitalWrite(dirPin, LOW);
  delay(1);
  //moveToEndLimit();
  while(digitalRead(9) != 1){
    digitalWrite(stepPin, HIGH);
    delay(1);
    digitalWrite(stepPin, LOW);
    delay(1);
  }
  minPos = analogRead(A0);
  Serial.println("finished first cal");
  digitalWrite(dirPin, HIGH);
  delay(1);
  //moveToEndLimit();
  while(digitalRead(10) != 0){
    digitalWrite(stepPin, HIGH);
    delay(1);
    digitalWrite(stepPin, LOW);
    delay(1);
  }
  maxPos = analogRead(A0);
  middlePosition = (maxPos + minPos)/2;
  Serial.println("midle pos:");
  Serial.println(middlePosition);
  Serial.println(minPos);
  Serial.println(maxPos);
  digitalWrite(dirPin, LOW);
  stepForward(1500, 800);
  delay(2000);

  steps = 0;
}


void loop() {

  while(digitalRead(9) == 1 and analogRead(A0) > middlePosition){
    Serial.println("left is pressed");
    digitalWrite(leftLED, HIGH);
    delay(5);
  }

  while(digitalRead(10) == 0 and analogRead(A0) < middlePosition){
    Serial.println("right is pressed");
    digitalWrite(rightLED, HIGH);
    delay(5);
  }

  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);

  int pot = analogRead(A0);
  

  int error = pot - middlePosition; // + (analogRead(A1)-512)/20;
  //Serial.println(steps);
  if(abs(error) < 5) {
    //in stable equilibrium
    
    amountToKick = KpKick * steps * -1; 
    
  }
  else{
    amountToKick = 0;
  }

  amountToMove = error * Kp + amountToKick; // + (Kd * (error - errorPrev) / dt);

  steps = steps + amountToMove;

  if(amountToMove > 0){
    digitalWrite(dirPin, 0);
    stepForward(amountToMove, 600);
  }
  else{
    digitalWrite(dirPin, 1);
    stepForward(-1*amountToMove, 600);
  }  
}

void stepForward (int steps, int delayTime) {
  for(int i=0; i<steps; i++){
    
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
    if(amountToMove > 0){
      amountToMove--;
    }
    else{
      amountToMove++;
    }
  }
}
