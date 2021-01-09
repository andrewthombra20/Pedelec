#include <RotaryEncoder.h>
#include <Ramp.h>                       
#include <VescUart.h>

VescUart UART;
rampFloat myRamp; 

RotaryEncoder encoder(A2, A3);

int previousDirection = 0;
int currentDirection = 0;
int newPos = 0;

float dutyCycle = 0;
float dutyCycleToVesc = 0;

double timeAtPulseDetection = 0;
double previousPulseDetectedTime = 0;
double pedalStopTime = 0;
double timeDifference = 0;
double rpm = 0;
double currentFilteredRPM = 0;
double previousFilteredRPM = 0;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  while (!Serial1) {;}
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);
} // setup()

void loop()
{
  static int pos = 0;
  encoder.tick();
  pedalStopTime = millis();
  newPos = encoder.getPosition();
  currentDirection = (int)encoder.getDirection(); 
  if (pos != newPos) 
  {
    timeAtPulseDetection= millis();
    timeDifference = timeAtPulseDetection-previousPulseDetectedTime; 
    pos = newPos;
    previousPulseDetectedTime=timeAtPulseDetection; 
    previousDirection = currentDirection;
  } 
  if ( (previousDirection != currentDirection) || ( pedalStopTime/1000 - timeAtPulseDetection/1000  > 0.45 ) )
  {
    rpm = 0;
    UART.setDuty(0);
  }
  else 
  {
    if ( timeDifference == 0 )
    {                                     
      rpm = 0;
    }
    else if ( currentDirection == 1)
    {                                            
      rpm = -36*1000*0.16667/timeDifference;                           
    }
    else if (currentDirection ==-1)
    {
      rpm = 36*1000*0.16667/timeDifference;
    }
    currentFilteredRPM  = 0.2*rpm + (1-0.2)* previousFilteredRPM;
    previousFilteredRPM = currentFilteredRPM;
    
    if(currentDirection == -1)
    {
          dutyCycleToVesc =mapLinear(currentFilteredRPM,-100,0,-0.1,-0.001);  
    }
    else if (currentDirection == 1)
    {
          dutyCycleToVesc =mapExponential(currentFilteredRPM);    
    }
    UART.setDuty(dutyCycleToVesc);
  }
} // loop ()


double mapLinear(float x, float in_min, float in_max, float out_min, float out_max)        // Linear mapping function to map rpm to duty cycle 
 {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 }

double mapExponential(float x)          //Exponential mapping function to map rpm to duty cycle
  {      
  float rpmAverage = 60 ;
  float maxDutyCycle = 0.5;
  return maxDutyCycle * (1 - (pow(2.7183,(-x/rpmAverage )))); 
   }
