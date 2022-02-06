#include <Servo.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myStepper = AFMS.getStepper(200, 2);

//use Servo2 as button pin
int buttonPin = 9;
//create int to loop through states
int state = 0;
//int motorSpeed = 90;                  //90 is stop for continuous servos
//global checker for angle, at center
int stepperDirection = 0;
int maxAngle = 50;
int minAngle = -50;
//forces stepper to turn left
bool turnLeft;
//forces one button press per swap
//int lastButtonState = 0;
int timer = 0;

Servo servo1;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps


  AFMS.begin();        // create with the default frequency 1.6KHz
  
  // Attach a servo to pin #10
  servo1.attach(10);
  servo1.write(90);
  // turn on motor M1

  // setup the stepper
  myStepper->setSpeed(10);

  //setup physical test light
  pinMode(LED_BUILTIN, OUTPUT);
  
  TCCR0A=(1<<WGM01);    //Set the CTC mode   
  OCR0A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK0|=(1<<OCIE0A);   //Set the interrupt request
  sei(); //Enable interrupt
  
  TCCR0B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR0B|=(1<<CS00);
}

void moveStepper(){
  if(state != 0){               //check if motor is not stopped
    switch(turnLeft){
      case true:                //if we are turning left, try to turn left
        if(stepperDirection == minAngle){           //if we hit the max value of turning left, set to false and do nothing else
          turnLeft = false;
        }
        else{
          myStepper->step(1, BACKWARD, MICROSTEP);     //else, step and decriment our current direction
          stepperDirection--;
        }
        break;
      default:                                      //turnLeft == false
        if(stepperDirection == maxAngle){           //if we hit the max value of turning left, set to false and do nothing else
          turnLeft = true;
        }
        else{
          myStepper->step(1, FORWARD, MICROSTEP);      //else, step and incriment our current direction
          stepperDirection++;
        }
        break;
    }
  }
}

//int i;
void loop() {
  int prevState = state;
  
  if(timer >= 2000){
    digitalWrite(LED_BUILTIN, HIGH);
    if(digitalRead(buttonPin) == 1){
      state++;
      //state = state % 4;
      timer = 0;                                                        //force only one button press per state change
      Serial.println(state);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
  if(prevState != state){
    switch(state){
      case(1):
        Serial.println("Starting...");
        myStepper->setSpeed(20);
        servo1.write(100);
        break;
      case(2):
        servo1.write(110);
        break;
      case(3):
        servo1.write(135);
        break;
      default:
        servo1.write(90);
        delay(20);
        state = 0;
        break;
    }
  }
 moveStepper();
  
 //delay(500);
}

ISR(TIMER0_COMPA_vect){    //This is the interrupt request
  timer++;
}
