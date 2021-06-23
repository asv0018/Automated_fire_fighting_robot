#include <Servo.h>  //include servo.h library
#include <SoftwareSerial.h>

SoftwareSerial gsm(2,3); // Rx, Tx on arduino
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

#define BUZZER 11

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
  pinMode(BUZZER, OUTPUT);
  myservo.attach(servo);
  myservo.write(90);
  gsm.begin(9600);
  Serial.begin(9600);
  init_sms();
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
    send_sms();
    for (pos = 30; pos <= 150; pos += 1) { 
    myservo.write(pos); 
    delay(20);  
  }
  for (pos = 150; pos >= 30; pos -= 1) { 
    myservo.write(pos); 
    delay(20);
  }
  digitalWrite(BUZZER,LOW);
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
      digitalWrite(BUZZER,HIGH);
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
  gsm.println("AT+CMGF=1");
  delay(30);
 }

void send_sms(){
 gsm.println("AT+CMGS=\"+918217520507\"");   // use your 10 digit cell no. here
 delay(30);
 gsm.write("ALERT!!!, Fire is detected!.");
 gsm.write(26);
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
