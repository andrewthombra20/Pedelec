// Vesc - Arduino setup to set duty cycle for BLDC motor based on the input from PAS sensor 
// using UART communication protocol. Pins 2 and 3 are used for Encoder sensor signals A and B respectively.
// Serial pins are used to communicate with the VESC 

#include <digitalWriteFast.h>         
#include <VescUart.h>

#define PI 3.1415926535897932384626433832795
#define encoderA 2
#define encoderB 3

VescUart UART;

volatile int compare=0;
volatile int countA = 0;
volatile int countB = 0;
volatile int cumulativeCountA = 0;
volatile int cumulativeCountB = 0;
volatile int directionold=0;
 

volatile int Dir = 0;  // 1 = CW
                       // 0 = Stationary
                       // -1 = CCW

boolean motion = false;

double t1=0;
double t2=0;
double t3=0;

float dt;
float rpmtovesc=0;
float dutycycle=0;
float RPM=0;

void setup() {
  Serial.begin(115200);                                                       // setting baud rate
  Serial1.begin(115200);

  pinMode(encoderA, INPUT);                                                   // defining Signal A as input at Pin encoderA
  pinMode(encoderB, INPUT);                                                   // defining Signal B as input at Pin encoderB
  
  attachInterrupt(digitalPinToInterrupt(encoderA), pulseA, RISING);           //Interrupt call when pulse is rising at Signal A
  while (!Serial1) {;}
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);
}                                                                            // end of setup loop 

void loop() {
  
  UART.setDuty(0);                               // passing parameter to function to set Duty cycle for the BLDC motor

  while(motion){                                 // loop active as long as pulses are detected
  
     
      if (dt==0){                                       // calculation of RPM
        RPM=0;
      }
      else {                                            // For RPM calculation when consecutive pulses are detected, a 30 degree change and time difference of dt
        RPM = 30/ (dt );                              // RPM = 30 / dt ;   (dt units are in milliseconds)
      }                                                    //
      
      t3=(millis());                                  // time variable to check if pedalling has stopped for more than 0.25 seconds
      //Serial.println(RPM);
      if (((t3/1000)-(t1/1000))>0.25){                   // Condition : when pedals stop rotating for more than 0.25 seconds, reset function is called
        reset();  
        motion = false;
        break;  
      }                                             //
      
      if (RPM > 1.8){                                  // limiting RPM in the range 0to 1.8 
        RPM = 1.8;        
      }
    
      if (compare==1){           //  RPM is passed as negative or positive depending on which direction is currently detected
        rpmtovesc=RPM;        //RPM positive if CW direction
      }
      else if (compare==-1){
        rpmtovesc=-RPM;       //RPM negative if CCW direction
      }
      else{
        rpmtovesc=0;    
      }
      dutycycle =vescdutymap(rpmtovesc,-1.8,1.8,-0.4,0.4);           // mapping function to map rpm to duty cycle
    
    //---------------------------------serial communication to vesc-----------------------------------//

   UART.setDuty(dutycycle);                             // passing parameter to function to set the Duty cycle for the BLDC motor
    }           // end of while loop

}                                                       // end of loop

//------------------------------         Funtions   -------------------------------------------------//

void checkDirection(){                                                                     // Funtion returns direction where 1 is CW and -1 is CCW directions
if((bool) digitalReadFast(encoderB) ==  HIGH){                                             //digitalReadFast() is faster than digitalRead()
    Dir = 1;  
  }
  else{
    Dir = -1;
  } 
compare=Dir;
}                                                                                            // 

void pulseA(){                                                                              // Interrupt function when pulse is rising
  checkDirection();                                                                         // checks direction to increment pulse in respective direction
  t2=millis();
  dt=t2-t1;                                                                                   
  countA += Dir;                                                                            
  cumulativeCountA += Dir;
  t1=t2;
  motion = true;
}                                                                                           //

void reset(){                                                                                 // reset function when no detecting of pulses
  countA = 0;   //reset counters at "home" reference point                                      
  countB = 0;                                                                                 
  RPM=0;
  t3=0;
  compare=0;
}                                                                                             //

double vescdutymap(float x, float in_min, float in_max, float out_min, float out_max) {       // mapping function to map RPM to duty cycle in this case
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 }                                                                                            //
