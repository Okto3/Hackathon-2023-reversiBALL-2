const int trigPin = 9;
const int echoPin = 10;
int time = 0;

long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance based on the speed of sound
  distance = duration * 0.034 / 2;

  // blinks LED at a frequency proportional to the distance
  // only starts flashing once it its <1m away
  if(distance < 100) {
    // distance is measured in cm, so for a distance of 1m, it will blink on for 1s and off for 1s
    time = 10 * distance;
    digitalWrite(2,HIGH);
    delay(time);
    digitalWrite(2, LOW);
    delay(time);

  }
}