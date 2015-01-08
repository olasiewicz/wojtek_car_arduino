  /*
  Author: Wojciech Olasiewicz
  TMwSS
*/

#include <Servo.h>
Servo servohead;

#define DIRECTION_R 13
#define SPEED_R 11
#define BREAK_R 8

#define DIRECTION_L 12
#define SPEED_L 3
#define BREAK_L 9

#define trigPin 4
#define echoPin 5
#define left 2
#define right 3
#define wartoscmin 0
#define wartoscmax 600

float dangerDistance = 25;
float distanceRight;
float distanceLeft;
int tonePin = 4;
int reverseLed = 7;
char val;

void setup() {

  pinMode(DIRECTION_R, OUTPUT);
  pinMode(SPEED_R, OUTPUT);
  pinMode(BREAK_R, OUTPUT);
  pinMode(DIRECTION_L, OUTPUT);
  pinMode(SPEED_L, OUTPUT);
  pinMode(BREAK_L, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(reverseLed, OUTPUT);
  pinMode(echoPin, INPUT);
  servohead.attach(6);
  
  Serial.begin(9600);
}


struct Pomiary
{
  int lewy;
  int prawy;
};


Pomiary odczyt;

void zmierz(Pomiary &odczyt);
void steruj(Pomiary odczyt);


void lightCommand()
{
  while (val != 'X')
  {
  val = Serial.read();
  zmierz(odczyt);
  steruj(odczyt);
  delay(100);
  } 
}


void zmierz(Pomiary &odczyt)
{
  odczyt.lewy = map(analogRead(left), 0, 1023, wartoscmin, wartoscmax);
  odczyt.prawy = map(analogRead(right), 0, 1023, wartoscmin, wartoscmax);
}


void steruj(Pomiary odczyt)
{
  if(odczyt.lewy == odczyt.prawy)
  {
    analogWrite(SPEED_R, 255);
    analogWrite(SPEED_L, 255);
    digitalWrite(DIRECTION_R, HIGH);
    digitalWrite(DIRECTION_L, HIGH);
  }
  
  if(odczyt.lewy < odczyt.prawy)
  {
    analogWrite(SPEED_R, 0);
    analogWrite(SPEED_L, 255);
    digitalWrite(DIRECTION_R, HIGH);
    digitalWrite(DIRECTION_L, HIGH); 
  }
  if(odczyt.lewy > odczyt.prawy)
  {  
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 0);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, HIGH);  
  }
}


void stopCommand()
{
   while (val != 'p')
  {
  val = Serial.read();
  analogWrite(SPEED_R, 0);
  analogWrite(SPEED_L, 0);
  digitalWrite(reverseLed, LOW);
  }
}


void ultraCommand()
{
  while (val != 'X')
  {
    val = Serial.read();
    float distanceRead;
    distanceRead = scan();
    startRobot();
    if (distanceRead < dangerDistance) 
    {
      stopRobot();  
      avoid();
    }

    else
    {
      forward();
    }
  } 
}


int scan()
{
  float dlugosc, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dlugosc = pulseIn(echoPin, HIGH);
  cm = (dlugosc / 2) / 29.1 ;
  return cm;
}


void avoid() 
{
  robotLookRight();
  distanceRight = scan();
  robotLookLeft();
  distanceLeft = scan();
  robotLookForward(); 
  if (distanceLeft < distanceRight) 
  {
    turnRight();
    delay(500);
  }
  else 
  {
    turnLeft();
    delay(500);
  }
}


void stopRobot()
{ 
  digitalWrite(BREAK_R, HIGH);
  digitalWrite(BREAK_L, HIGH);
  analogWrite(SPEED_R, 0);
  analogWrite(SPEED_L, 0);
}



void startRobot()
{
  servohead.write(90);
  digitalWrite(BREAK_L, LOW);
  digitalWrite(BREAK_R, LOW);
}


void forward()
{
    analogWrite(SPEED_R, 255);
    analogWrite(SPEED_L, 255);
    digitalWrite(DIRECTION_R, HIGH);
    digitalWrite(DIRECTION_L, HIGH);
    digitalWrite(BREAK_L, LOW);
    digitalWrite(BREAK_R, LOW);
}


void robotLookForward()
{
  servohead.write(90);
}

 
void robotLookLeft()
{
  servohead.write(150);
  delay(500);
}


void robotLookRight()
{
  servohead.write(30);
  delay(500);
}


void turnLeft()
{
  analogWrite(SPEED_R, 220);
  analogWrite(SPEED_L, 220);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, LOW); 
  digitalWrite(BREAK_L, LOW);
  digitalWrite(BREAK_R, LOW); 
}


void turnRight()
{
  analogWrite(SPEED_R, 220);
  analogWrite(SPEED_L, 220);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, HIGH);  
  digitalWrite(BREAK_L, LOW);
  digitalWrite(BREAK_R, LOW);
}


void accelerometerCommand() 
{
  while(val != 'X')
  {
  val = Serial.read(); 
  if (val == 'f')
  {
    go_forward();
  } 
  
   else if (val == 'F')
   { 
     go_forwardFast();
   }
    
   else if (val == 'b')
   {
     go_reverse();
   } 
   
   else if (val == 'r') 
   { 
     go_right();
   } 
    
   else if (val == 'l') 
   { 
     go_left();
   } 
   
   else if (val == 'R') 
   { 
     go_rightFast();
   } 
    
   else if (val == '>') 
   { 
     go_right_b();
   } 
    
   else if (val == '}') 
   { 
     go_rightFast_b();
   } 
    
   else if (val == 'L') 
   { 
     go_leftFast();
   } 
    
    else if (val == '<') 
   { 
     go_left_b();
   } 
   
   else if (val == '{') 
   { 
     go_leftFast_b();
   } 
   
   else if (val == 's') 
   { 
     go_stop();
   } 
  }
}


void go_forward() 
{
  analogWrite(SPEED_R, 180);
  analogWrite(SPEED_L, 180);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, LOW);
}


void go_forwardFast() 
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, LOW);
}


void go_reverse() 
{
  analogWrite(SPEED_R, 240);
  analogWrite(SPEED_L, 240);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, LOW);
  digitalWrite(reverseLed, HIGH);
}


void go_stop() 
{
  analogWrite(SPEED_R, 0);
  analogWrite(SPEED_L, 0);
  digitalWrite(reverseLed, LOW);
}


void go_left()
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 180);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, LOW);
}


void go_right() 
{
  analogWrite(SPEED_R, 180);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, LOW);
}


void go_leftFast()
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, LOW);
  digitalWrite(reverseLed, LOW);
}


void go_rightFast() 
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, LOW);
}


void go_right_b()
{
  analogWrite(SPEED_R, 180);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, LOW);
  digitalWrite(reverseLed, HIGH);
}


void go_rightFast_b()
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, HIGH);
  digitalWrite(DIRECTION_L, LOW);
  digitalWrite(reverseLed, HIGH);
}


void go_left_b()
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 180);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, LOW);
  digitalWrite(reverseLed, HIGH);
}


void go_leftFast_b()
{
  analogWrite(SPEED_R, 255);
  analogWrite(SPEED_L, 255);
  digitalWrite(DIRECTION_R, LOW);
  digitalWrite(DIRECTION_L, HIGH);
  digitalWrite(reverseLed, HIGH);
}


void loop()
{
  if(Serial.available())
  {
    val = Serial.read();
  }
  
  if(val == 'A')
  {
    accelerometerCommand();
  }
  else if(val == 'U')
  {
    ultraCommand();
  }
  
  else if(val == 'S')
  {
    lightCommand();
  }
  
  else if(val == 'X')
  {
    stopCommand();
  }
}

