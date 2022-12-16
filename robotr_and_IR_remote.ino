/*Robot car controlled with IR remote
 * This program causes the remote to have 3 functions that can be activated by pressing 1,2 or 3
 * 1: activates the mode where the robot car can be controlled by remote
 * 2: activates a line tracking method, where the robot car follows a black line with the use of light sensors
 * 3: activates avoidance mode, where the robot car moves continuously until the ultrasonic senses an obejct and the moves away
 */


#include "IRremote.h"//include IR remote library

/*----- Variables, Pins -----*/

//time
unsigned long current_millis = millis();//assigning a long variable to millis
unsigned long previous_millis; // assigning this variable for the use in a loop without using delay

//LIGHT SENSORS
int sensor_right;//variable data light sensor right
int sensor_left;//variable data light sensor left
int sensor_middle;/variable data light sensor middle


//IR
int receiver = 9; // Signal Pin of IR receiver to Arduino Digital Pin 6
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

//MODE
int mode = 0;// to swithc between all 3 functionalities

//ULTRASONIC
#define trigPin 13//defines the output ultrasonic value pin
#define echoPin 12//defines the input ultrasonic value pin 
float duration, distance;//setting up variables for the duration before the wave is received and calculates the distance


//SERVO
#include <Servo.h>//add servo library
Servo myservo;  // create servo object to control a servo


void setup()// this code runs once
{ 
  //robot
  for(int i=0; i<9; i ++){// a for loop hapenning 9 times
    pinMode(i,OUTPUT);// setting the pins 0-8
  }
  digitalWrite(3, HIGH);//this pin has to be on for the robot to work

  //IR
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600); // starts serial monitor for easy reading of ir values

  //ultrasonic
    pinMode(trigPin, OUTPUT);// setting pin 10 as an output
  pinMode(echoPin, INPUT);//setting pin 13 as an input

  //servo
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
}


//robot move----------------------------------------
void move(int a, int b, int c, int d){// a function to make it easier to move the robot
  digitalWrite(7,a);//writes value a to pin 7
  analogWrite(5,b);//writes value b to pin 5
  digitalWrite(8,c);//writes value c to pin 8
  analogWrite(6,d);//writes value d to pin 6
}

void forward(int x){//function for forward moving
  move(1,x,1,x); // turns on all motors at x speed
}
void left(int x){//function for left moving
  
  move(1,x,0,0); // turns on left motors at x speed
}
void right(int x){//function for right moving
  move(0,0,1,x); // turns on right at x speed
}
void right_onplace(int x){//function for right_onplace moving
  move(0,x,1,x); // turns on all motors at x speed, right forward and left backwards
}
void left_onplace(int x){//function for left_onplace moving
  move(1,x,0,x); // turns on all motors at x speed, left forward and right backwards
}
void left_forward(int x){//function for left_forward moving
  
  move(1,x,1,x/2);   // turns on left at x speed, and right at half x speed
}
void right_forward(int x){//function for right_forward moving
  move(1,x/2,1,x);// turns on right at x speed, and left at half x speed
}
void backward(int x){//function for backward moving
  move(0,x,0,x); // turns on all motors at x speed backwards
}
void stop(){//function for stopping
  move(0,0,0,0); // turns off everything
} 

//ULTRASONIC
void read(){//function for reading the values to avoid a lengthy program
  // Write a pulse to the HC-SR04 Trigger Pin 
  digitalWrite(trigPin, LOW);//sets pin 10 off
  delayMicroseconds(2);//delays for 2 microseconds
  digitalWrite(trigPin, HIGH);;//sets pin 10 on
  delayMicroseconds(10);//delays for 10 microseconds
  digitalWrite(trigPin, LOW);;//sets pin 10 off
  duration = pulseIn(echoPin, HIGH);;//reads the value returned from pin 13 and stores it in duration(of wave)
  distance = (duration / 2) * 0.0343;//converts duration into distance in metric centimeters
}



void loop()//this code runs repeatidly
{

  if(mode == 0){//if there is no mode active the robot stops
    stop();
  }
  
  //IR
if (irrecv.decode(&results)) // have we received an IR signal?

  {
    switch(results.value)//does a case depending on results.value from IR

    {
      if(mode == 1){//if mode is 1
      case 16736925: // UP button pressed
      forward(100);//move forward at 100 speed
                      
                      
                      break;//leave switch
      case 16720605: // LEFT button pressed
      left_onplace(100);//move left onplace at 100 speed
                      
                      
                      break;//leave switch

      case 16754775: // DOWN button pressed
       backward(100);//move backwards at 100 speed
                      
                      
                      break;//leave switch
      case 16761405: // RIGHT button pressed
       right_onplace(100);//move right onplace at 100 speed
                      
                      
                      break;//leave switch
      }
      
      case 16738455: // 1 button pressed
      mode = 1;// set mode to 1
                      
                      
                      break;//leave switch
      case 16750695: // 2 button pressed
      mode = 2;// set mode to 2
                      
                      
                      break;//leave switch

      case 16756815: // 3 button pressed
       mode = 3;// set mode to 3
       break;//leave switch
      case 16712445: // OK button pressed
       mode = 0;// set mode to 0
       break;//leave switch



                      
    }
      Serial.println(results.value);//prints values in seral monitor to make it easy to figure at what IR signal each buttone sends
      irrecv.resume(); // receive the next value
  } 



  //LINE FOLLOW
  if(mode == 2){//if mode is 2
     current_millis = millis();//resetting the variable to time since the robot has started
  sensor_right = analogRead(A0);//stores reading from sensor right
  sensor_left = analogRead(A2);//stores reading from sensor left
  sensor_middle = analogRead(A1);//stores reading from sensor middle
  // put your main code here, to run repeatedly:
 if(sensor_middle >50){// if the middle sensor see's black
  forward(75);//move forward at 100 speed
 }
 else if(sensor_right >50){//if the right sensor senses black
  right(250);//move right at 100 speed

 }
 else if(sensor_left >50){//if the left sensor senses black
  left(250);//move left at 100 speed


 }
 else{//otherwise

  //search
  previous_millis = millis();//setting the variable to time since the program started
   while((current_millis-previous_millis) <100){//this goes until 100 ms has passed
    current_millis = millis();//set's the variable to time since the program started
      right_onplace(100);//move right onplace at 100 speed
      if(sensor_middle >50){// if the middle sensor see's black it stops search
        break; //leaves search loop
      }
   }

 }
  }
  //ULTRASONIC
  else if(mode == 3){// if mode is 3 it does the followring

       forward(100);//move forward at 100 speed

   
   myservo.write(90);//set's the servo forward                 

    read();//reads ultrasonic vaules
   
  if (distance <= 20) {//all checks if distance is less then 20 cm
    stop();//stops robot
    myservo.write(180);//set's the servo left

     delay(1000); //delay 1 sec to give the servo time to move
     read(); //reads ultrasonic vaules
     myservo.write(90); //set's the servo forward   
     if (distance <= 20) {//all checks if distance is less then 20 cm
     myservo.write(0);//set's the servo right
     delay(1000);//delay 1 sec to give the servo time to move
     read();//reads ultrasonic vaules
     myservo.write(90);//set's the servo forward   
       if (distance <= 20) {//all checks if distance is less then 20 cm
    left_onplace(100);//move  at 100 speed
     delay(1200);//delay 1.2 sec
  }
  else{
    right_onplace(100);//move right at 100 speed
     delay(600);//delay 0.6 sec
  }
  }
  }
  else{
    left_onplace(100);//move  at 100 speed
    delay(600);//delay 0.6 sec
  }

  }
  }


}/* --end main loop -- */
