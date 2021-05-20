/*------ Arduino Fire Fighting Robot Code by hobby project---- */
 
#include <Servo.h>  //include servo.h library
Servo myservo;

int pos = 0;    
boolean fire = false;

const int trigPin = A0;
const int echoPin = A1;

#define Left A4      // left sensor
#define Right A2    // right sensor
#define Forward A3   //front sensor

#define LM1 6      // left motor
#define LM2 5       // left motor
#define RM1 8       // right motor
#define RM2 7       // right motor

#define servo 9

#define pump 4
 
void setup(){
  pinMode(pump, INPUT_PULLUP);
  //pinMode(pump, OUTPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(servo);
  myservo.write(90); 
}
 
void put_off_fire() {
    delay(200);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
   pinMode(pump,OUTPUT);
   digitalWrite(pump, LOW);
   delay(500);
    
    for (pos = 50; pos <= 130; pos += 1) { 
    myservo.write(pos); 
    delay(10);  
  }
  for (pos = 130; pos >= 50; pos -= 1) { 
    myservo.write(pos); 
    delay(10);
  }
  
  digitalWrite(pump,HIGH);
  myservo.write(90);
  
  fire=false;
}
 
void loop(){
    myservo.write(90);
    
    if (digitalRead(Left) ==1 && digitalRead(Right)==1 && digitalRead(Forward) ==1){
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    }
    
    else if (digitalRead(Forward) ==0){
    long dist = measure_obstacle_distance();
    if (dist>15){
      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);
      fire = true;
      }
    }
    
    else if (digitalRead(Left) ==0){
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    }
    
    else if (digitalRead(Right) ==0){
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    }
    
    delay(30);//change this value to increase the distance
    
     while (fire == true){
      put_off_fire();
     }
     
}


void init_sms(){
  Serial.println("AT+CMGF=1");
  delay(30);
  Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"");   // use your 10 digit cell no. here
  delay(30);
 }

void send_sms(){
 Serial.write(26);
}

long measure_obstacle_distance(){
  long duration;
  int distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
  
}
